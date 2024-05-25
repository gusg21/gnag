#ifndef AUDIO_H
#define AUDIO_H

#include <3ds.h>
#include <opus/opusfile.h>

#define AUDIO_SAMPLE_RATE 48000                              // 48k
#define AUDIO_SAMPLE_COUNT ((u16)((120.f / 1000.f) * (float)AUDIO_SAMPLE_RATE))  // Calculate the samples in 120 milliseconds
#define AUDIO_CHANNELS_PER_SAMPLE 2                          // stereo
#define AUDIO_WAVE_BUFFER_SIZE ((u16)AUDIO_SAMPLE_COUNT) * ((u16)AUDIO_CHANNELS_PER_SAMPLE) * ((u16)sizeof(s16))
#define AUDIO_NUM_WAVE_BUFFERS 3

#define AUDIO_THREAD_CORE -1               // any core
#define AUDIO_THREAD_STACK_SIZE 32 * 1024  // 32K stack

#define AUDIO_MAX_AUDIO_INSTANCES 64
#define AUDIO_NUM_INSTANCE_CHANNELS 23

struct audio_s;

typedef struct {
    bool playing;
    ndspWaveBuf wave_bufs[AUDIO_NUM_WAVE_BUFFERS];
    s16* audio_buf;
    Thread decoding_thread_id;
    OggOpusFile* opus_file;
    u32 channel;
    struct audio_s* audio;
} audio_instance_t;

typedef struct audio_s {
    audio_instance_t instances[AUDIO_MAX_AUDIO_INSTANCES];
    u32 next_instance_index;
    u32 playing_channels;
    LightEvent event;
} audio_t;

void Audio_Init(audio_t* audio);
audio_instance_t* Audio_Play(audio_t* audio, const char* opus_path);
u32 Audio_FindOpenChannel(audio_t* audio);
void Audio_FlagChannelAsPlaying(audio_t* audio, u32 channel_index);
void Audio_FlagChannelAsNotPlaying(audio_t* audio, u32 channel_index);
void Audio_Destroy(audio_t* audio);

#endif  // AUDIO_H