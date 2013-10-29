#ifndef STATIC_LINK
#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif


#include <hx/CFFI.h>

#include "AudioEngine.h"
#include "ModSound.h"
#include "VorbisSound.h"
#include "FlacSound.h"
using namespace Audaxe;

vkind kSound;
vkind kEngine;
vkind kChannel;

enum SoundOperation {
    kSoundOperationPlay = 0,
    kSoundOperationStop,
    kSoundOperationPause,
    kSoundOperationResume
};

/**
 * ENGINE
 */

void audaxe_engine_close(value engineHandle)
{
    AudioEngine *engine = (AudioEngine *)val_to_kind(engineHandle, kEngine);
    delete engine;
    engine = 0;
}

value audaxe_engine_init()
{
    AudioEngine *engine = new AudioEngine();
    value v = alloc_abstract(kEngine, engine);
    val_gc(v, audaxe_engine_close);
    return v;
}

value audaxe_engine_volume(value volume)
{
    if (val_is_float(volume))
    {
        AudioEngine::volume = val_float(volume);
    }
    return volume;
}

value audaxe_engine_create_channel(value engineHandle)
{
    if (!val_is_kind(engineHandle, kEngine)) return alloc_null();

    AudioEngine *engine = (AudioEngine *)val_to_kind(engineHandle, kEngine);
    AudioChannel *channel = engine->createChannel();
    return alloc_abstract(kChannel, channel);
}

DEFINE_PRIM(audaxe_engine_init, 0);
DEFINE_PRIM(audaxe_engine_volume, 1);
DEFINE_PRIM(audaxe_engine_create_channel, 1);

/**
 * SOUND
 */

value audaxe_sound_load_tracker(value filename)
{
	ModSound *sound = new ModSound(val_string(filename));
	sound->setLoopMode(true);
    return alloc_abstract(kSound, sound);
}

value audaxe_sound_load_vorbis(value filename)
{
    VorbisSound * sound = new VorbisSound(val_string(filename));
    return alloc_abstract(kSound, sound);
}

value audaxe_sound_load_flac(value filename)
{
    FlacSound * sound = new FlacSound(val_string(filename));
    return alloc_abstract(kSound, sound);
}

void audaxe_sound_operation(value soundHandle, value operation)
{
    if (val_is_kind(soundHandle, kSound))
    {
        AudioSound *sound = (AudioSound *)val_to_kind(soundHandle, kSound);
        switch (val_int(operation))
        {
            case kSoundOperationPlay: sound->play(); break;
            case kSoundOperationStop: sound->stop(); break;
            case kSoundOperationPause: sound->pause(); break;
            case kSoundOperationResume: sound->resume(); break;
        }
    }
}

void audaxe_sound_loop_points(value soundHandle, value start, value end)
{
    if (val_is_kind(soundHandle, kSound))
    {
        AudioSound *sound = (AudioSound *)val_to_kind(soundHandle, kSound);
        sound->setLoopPoints(val_int(start), val_int(end), kAudioModeMilliseconds);
        sound->setLoopMode(true);
    }
}

DEFINE_PRIM(audaxe_sound_load_tracker, 1);
DEFINE_PRIM(audaxe_sound_load_vorbis, 1);
DEFINE_PRIM(audaxe_sound_load_flac, 1);
DEFINE_PRIM(audaxe_sound_operation, 2);
DEFINE_PRIM(audaxe_sound_loop_points, 3);

/**
 * CHANNEL
 */

void audaxe_channel_set_sound(value channelHandle, value soundHandle)
{
    if (val_is_kind(channelHandle, kChannel) && val_is_kind(soundHandle, kSound))
    {
        AudioChannel *channel = (AudioChannel *)val_to_kind(channelHandle, kChannel);
        AudioSound *sound = (AudioSound *)val_to_kind(soundHandle, kSound);
        channel->setSound(sound);
    }
}

value audaxe_channel_volume(value channelHandle, value volume)
{
    if (val_is_kind(channelHandle, kChannel))
    {
        AudioChannel *channel = (AudioChannel *)val_to_kind(channelHandle, kChannel);
        channel->setVolume(val_float(volume));
    }
    return volume;
}

value audaxe_channel_pan(value channelHandle, value pan)
{
    if (val_is_kind(channelHandle, kChannel))
    {
        AudioChannel *channel = (AudioChannel *)val_to_kind(channelHandle, kChannel);
        channel->setPan(val_float(pan));
    }
    return pan;
}

value audaxe_channel_rate(value channelHandle, value rate)
{
    if (val_is_kind(channelHandle, kChannel))
    {
        AudioChannel *channel = (AudioChannel *)val_to_kind(channelHandle, kChannel);
        channel->setRate(val_float(rate));
    }
    return rate;
}

DEFINE_PRIM(audaxe_channel_set_sound, 2);
DEFINE_PRIM(audaxe_channel_volume, 2);
DEFINE_PRIM(audaxe_channel_pan, 2);
DEFINE_PRIM(audaxe_channel_rate, 2);

extern "C" void audaxe_main()
{
    int id_pan = val_id("pan"); // init neko...

    kSound = alloc_kind();
    kEngine = alloc_kind();
    kChannel = alloc_kind();
}
DEFINE_ENTRY_POINT(audaxe_main);

extern "C" int audaxe_register_prims () { return 0; }
