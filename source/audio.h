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

typedef struct {
    ndspWaveBuf wave_bufs[AUDIO_NUM_WAVE_BUFFERS];
    s16* audio_buf;
    LightEvent event;
    Thread decoding_thread_id;
    OggOpusFile* opus_file;
} audio_t;

void Audio_Init(audio_t* audio);
void Audio_Destroy(audio_t* audio);

#endif  // AUDIO_H