#ifndef __AUDAXE_H__
#define __AUDAXE_H__

#define kGlobalAudioBuffers			3
#define kGlobalAudioBufferFrames	1024
#define kGlobalAudioSampleRate		44100
#define kGlobalAudioChannels		2
#define kGlobalMaxChannels          32

#if defined(IPHONE) || defined(ANDROID)
    #define kAudioBits	16
#else
    #define kAudioBits	32
#endif

enum AudioMode
{
    kAudioModeBytes = 0,
    kAudioModeSamples,
    kAudioModeMilliseconds,
    kAudioModeSequence,
    kAudioModeLine
};

enum AudioStatus
{
    kAudioStatusComplete = -1,
    kAudioStatusStopped = 0,
    kAudioStatusPlaying
};

#endif // __AUDAXE_H__
