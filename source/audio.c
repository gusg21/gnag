#include "audio.h"

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <opus/opusfile.h>

#include "_defs.h"
#include "panic.h"

static const char* Audio_S_ErrorCodeToString(int error) {
    switch (error) {
        case OP_FALSE:
            return "OP_FALSE: A request did not succeed.";
        case OP_HOLE:
            return "OP_HOLE: There was a hole in the page sequence numbers.";
        case OP_EREAD:
            return "OP_EREAD: An underlying read, seek or tell operation "
                   "failed.";
        case OP_EFAULT:
            return "OP_EFAULT: A NULL pointer was passed where none was "
                   "expected, or an internal library error was encountered.";
        case OP_EIMPL:
            return "OP_EIMPL: The stream used a feature which is not "
                   "implemented.";
        case OP_EINVAL:
            return "OP_EINVAL: One or more parameters to a function were "
                   "invalid.";
        case OP_ENOTFORMAT:
            return "OP_ENOTFORMAT: This is not a valid Ogg Opus stream.";
        case OP_EBADHEADER:
            return "OP_EBADHEADER: A required header packet was not properly "
                   "formatted.";
        case OP_EVERSION:
            return "OP_EVERSION: The ID header contained an unrecognised "
                   "version number.";
        case OP_EBADPACKET:
            return "OP_EBADPACKET: An audio packet failed to decode properly.";
        case OP_EBADLINK:
            return "OP_EBADLINK: We failed to find data we had seen before or "
                   "the stream was sufficiently corrupt that seeking is "
                   "impossible.";
        case OP_ENOSEEK:
            return "OP_ENOSEEK: An operation that requires seeking was "
                   "requested on an unseekable stream.";
        case OP_EBADTIMESTAMP:
            return "OP_EBADTIMESTAMP: The first or last granule position of a "
                   "link failed basic validity checks.";
        default:
            return "Unknown error.";
    }
}

// Returns true -> still playing, false -> all done :)
static bool Audio_S_FillWaveBuffer(OggOpusFile* opus_file, ndspWaveBuf* wave_buffer, u32 channel) {
    // Decode samples until our waveBuf is full
    s32 total_samples = 0;
    while (total_samples < AUDIO_SAMPLE_COUNT) {
        s16* buffer = wave_buffer->data_pcm16 + (total_samples * AUDIO_CHANNELS_PER_SAMPLE);
        size_t bufferSize = (AUDIO_SAMPLE_COUNT - total_samples) * AUDIO_CHANNELS_PER_SAMPLE;

        // Decode bufferSize samples from opus_file into buffer,
        // storing the number of samples that were decoded (or error)
        s16 samples_or_error_code = op_read_stereo(opus_file, buffer, bufferSize);
        if (samples_or_error_code <= 0) {
            if (samples_or_error_code == 0) break;  // No error here

            Panic_Panic();
            CTR_PRINTF("op_read_stereo: error %d\n", samples_or_error_code);
            CTR_PRINTF("%s\n", Audio_S_ErrorCodeToString(samples_or_error_code));
            break;
        }

        total_samples += samples_or_error_code;
    }

    // If no samples were read in the last decode cycle, we're done
    if (total_samples == 0) {
        return false;
    }

    // Pass samples to NDSP
    wave_buffer->nsamples = total_samples;
    ndspChnWaveBufAdd(channel, wave_buffer);
    DSP_FlushDataCache(wave_buffer->data_pcm16, total_samples * AUDIO_CHANNELS_PER_SAMPLE * sizeof(int16_t));

    return true;
}

static void Audio_SoundFrameCallback(void* audio_ptr) {
    audio_t* audio = (audio_t*)audio_ptr;
    LightEvent_Signal(&audio->event);
}

static void AudioInstance_DecodingThread(void* audio_ptr) {
    audio_instance_t* audio = (audio_instance_t*)audio_ptr;

    while (audio->playing) {  // Whilst the quit flag is unset,
                              // search our waveBufs and fill any that aren't currently
                              // queued for playback (i.e, those that are 'done')

        for (size_t i = 0; i < AUDIO_NUM_WAVE_BUFFERS; ++i) {
            if (audio->wave_bufs[i].status != NDSP_WBUF_DONE) {
                continue;
            }

            if (!Audio_S_FillWaveBuffer(audio->opus_file, &audio->wave_bufs[i], audio->channel)) {  // Playback complete
                audio->playing = false;
                Audio_FlagChannelAsNotPlaying(audio->audio, audio->channel);
                return;
            }
        }

        // Wait for a signal that we're needed again before continuing,
        // so that we can yield to other things that want to run
        // (Note that the 3DS uses cooperative threading)
        LightEvent_Wait(&audio->audio->event);
    }
}

void Audio_Init(audio_t* audio) {
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);

    CTR_PRINTF("WAVE BUFFER SIZE: %d\n", AUDIO_WAVE_BUFFER_SIZE);
    CTR_PRINTF("AUDIO SAMPLE COUNT: %d\n", AUDIO_SAMPLE_COUNT);

    // Load more data event
    LightEvent_Init(&audio->event, RESET_ONESHOT);

    // Set the callback
    ndspSetCallback(Audio_SoundFrameCallback, audio);

    audio->next_instance_index = 0;
}

audio_instance_t* Audio_Play(audio_t* audio, const char* opus_path) {
    audio_instance_t* audio_instance = &audio->instances[audio->next_instance_index];
    audio->next_instance_index++;
    audio_instance->audio = audio;
    audio_instance->playing = true;
    audio_instance->channel = Audio_FindOpenChannel(audio);
    Audio_FlagChannelAsPlaying(audio, audio_instance->channel);
    CTR_PRINTF("Audio playing on channel %ld\n", audio_instance->channel);

    ndspChnReset(audio_instance->channel);
    ndspChnSetInterp(audio_instance->channel, NDSP_INTERP_POLYPHASE);
    ndspChnSetRate(audio_instance->channel, AUDIO_SAMPLE_RATE);
    ndspChnSetFormat(audio_instance->channel, NDSP_FORMAT_STEREO_PCM16);

    // Load the opus file
    int error = 0;
    audio_instance->opus_file = op_open_file(opus_path, &error);
    if (error != 0) {
        Panic_Panic();
        CTR_PRINTF("Failed to load opus file from %s\n", opus_path);
        CTR_PRINTF("%s\n", Audio_S_ErrorCodeToString(error));
        return NULL;
    }

    // Allocate audio buffer
    size_t buffer_size = AUDIO_WAVE_BUFFER_SIZE * AUDIO_NUM_WAVE_BUFFERS;
    audio_instance->audio_buf = (s16*)linearAlloc(buffer_size);
    if (!audio_instance->audio_buf) {
        Panic_Panic();
        CTR_PRINTF("Failed to allocate audio buffer\n");
        return NULL;
    }

    // Setup wavebufs
    memset(&audio_instance->wave_bufs, 0, sizeof(ndspWaveBuf) * AUDIO_NUM_WAVE_BUFFERS);
    for (size_t i = 0; i < AUDIO_NUM_WAVE_BUFFERS; i++) {
        audio_instance->wave_bufs[i].data_vaddr =
            audio_instance->audio_buf + (AUDIO_SAMPLE_COUNT * AUDIO_CHANNELS_PER_SAMPLE * i);
        audio_instance->wave_bufs[i].status = NDSP_WBUF_DONE;
    }

    // Spawn the decoding thread
    int32_t thread_priority = 0x30;
    svcGetThreadPriority(&thread_priority, CUR_THREAD_HANDLE);
    thread_priority--;  // Lower is higher priority
    // Clamp priority in valid range
    thread_priority = MIN(thread_priority, 0x3F);
    thread_priority = MAX(thread_priority, 0x18);

    audio_instance->decoding_thread_id =
        threadCreate(AudioInstance_DecodingThread, audio_instance, AUDIO_THREAD_STACK_SIZE, thread_priority,
                     AUDIO_THREAD_CORE, false);

    CTR_PRINTF("Thread ID %p\n", audio_instance->decoding_thread_id);

    return audio_instance;
}

u32 Audio_FindOpenChannel(audio_t* audio) {
    for (u32 channel_index = 0; channel_index < AUDIO_NUM_INSTANCE_CHANNELS; channel_index++) {
        bool playing = audio->playing_channels & (1 << channel_index);
        if (!playing) {
            return channel_index;
        }
    }
    return 0;
}

void Audio_FlagChannelAsPlaying(audio_t* audio, u32 channel_index) {
    u32 channel_mask = (1 << channel_index);
    audio->playing_channels |= channel_mask;
}

void Audio_FlagChannelAsNotPlaying(audio_t* audio, u32 channel_index) {
    u32 channel_mask = (1 << channel_index);
    audio->playing_channels &= ~channel_mask;
}

void Audio_Destroy(audio_t* audio) {
    UNUSED(audio);

    // threadJoin(audio->decoding_thread_id, UINT64_MAX);
    // threadFree(audio->decoding_thread_id);

    // ndspChnReset(0);
    // linearFree(audio->audio_buf);

    // op_free(audio->opus_file);
}
