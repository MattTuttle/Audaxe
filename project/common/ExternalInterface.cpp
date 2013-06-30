#ifndef IPHONE
#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif


#include <hx/CFFI.h>
#include <hxcpp.h>

#include "AudioEngine.h"
#include "ModSound.h"
#include "VorbisSound.h"
#include "FlacSound.h"
using namespace Audaxe;

DEFINE_KIND(k_Sound);
DEFINE_KIND(k_Engine);
DEFINE_KIND(k_Channel);

enum SoundOperation {
    kSoundOperationPlay = 0,
    kSoundOperationStop,
    kSoundOperationPause,
    kSoundOperationResume
};

/**
 * ENGINE
 */

value audaxe_engine_init()
{
    AudioEngine *engine = new AudioEngine();
    return alloc_abstract(k_Engine, engine);
}
DEFINE_PRIM (audaxe_engine_init, 0);

void audaxe_engine_volume(value volume)
{
    AudioEngine::volume = val_float(volume);
}
DEFINE_PRIM (audaxe_engine_volume, 1);

value audaxe_engine_create_channel(value engineHandle)
{
    val_check_kind(engineHandle, k_Engine);
    AudioEngine *engine = (AudioEngine *)val_data(engineHandle);
    AudioChannel *channel = engine->createChannel();
    return alloc_abstract(k_Channel, channel);
}
DEFINE_PRIM (audaxe_engine_create_channel, 1);

/**
 * SOUND
 */

value audaxe_sound_load_tracker(value filename)
{
	ModSound *sound = new ModSound(val_string(filename));
	sound->setLoopMode(true);
    return alloc_abstract(k_Sound, sound);
}
DEFINE_PRIM (audaxe_sound_load_tracker, 1);

value audaxe_sound_load_vorbis(value filename)
{
    VorbisSound * sound = new VorbisSound(val_string(filename));
    return alloc_abstract(k_Sound, sound);
}
DEFINE_PRIM (audaxe_sound_load_vorbis, 1);

value audaxe_sound_load_flac(value filename)
{
    FlacSound * sound = new FlacSound(val_string(filename));
    return alloc_abstract(k_Sound, sound);
}
DEFINE_PRIM (audaxe_sound_load_flac, 1);

void audaxe_sound_operation(value soundHandle, value operation)
{
    val_check_kind(soundHandle, k_Sound);
    AudioSound *sound = (AudioSound *)val_data(soundHandle);
    switch (val_int(operation))
    {
        case kSoundOperationPlay: sound->play(); break;
        case kSoundOperationStop: sound->stop(); break;
        case kSoundOperationPause: sound->pause(); break;
        case kSoundOperationResume: sound->resume(); break;
    }
}
DEFINE_PRIM (audaxe_sound_operation, 2);

void audaxe_sound_loop_points(value handle, value start, value end)
{
    val_check_kind(handle, k_Sound);
    AudioSound *sound = (AudioSound *)val_data(handle);
    sound->setLoopPoints(val_int(start), val_int(end), kAudioModeMilliseconds);
    sound->setLoopMode(true);
}
DEFINE_PRIM (audaxe_sound_loop_points, 3);

/**
 * CHANNEL
 */

void audaxe_channel_set_sound(value channelHandle, value soundHandle)
{
    val_check_kind(channelHandle, k_Channel);
    AudioChannel *channel = (AudioChannel *)val_data(channelHandle);
    AudioSound *sound = (AudioSound *)val_data(soundHandle);
    channel->setSound(sound);
}
DEFINE_PRIM (audaxe_channel_set_sound, 2);

void audaxe_channel_volume(value channelHandle, value volume)
{
    val_check_kind(channelHandle, k_Channel);
    AudioChannel *channel = (AudioChannel *)val_data(channelHandle);
    channel->setVolume(val_float(volume));
}
DEFINE_PRIM (audaxe_channel_volume, 2);

void audaxe_channel_pan(value channelHandle, value pan)
{
    val_check_kind(channelHandle, k_Channel);
    AudioChannel *channel = (AudioChannel *)val_data(channelHandle);
    channel->setPan(val_float(pan));
}
DEFINE_PRIM (audaxe_channel_pan, 2);

void audaxe_channel_rate(value channelHandle, value rate)
{
    val_check_kind(channelHandle, k_Channel);
    AudioChannel *channel = (AudioChannel *)val_data(channelHandle);
    channel->setRate(val_float(rate));
}
DEFINE_PRIM (audaxe_channel_rate, 2);

extern "C" int audaxe_register_prims () { return 0; }