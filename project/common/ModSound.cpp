/*
 *  ModPlayer.cpp
 *
 *  Created by Matthew Tuttle on 7/12/10.
 *  Copyright 2010 Frozen Orange, Inc. All rights reserved.
 *
 */

#include <dumb.h>
#include "ModSound.h"

namespace Audaxe
{

	int loop_callback(void *data) {
		ModSound* mp = static_cast<ModSound*>(data);
		if(mp->getLoopMode() == false)
		{
			mp->seek(0, kAudioModeSamples);
			mp->setStatus(kAudioStatusStopped);
			return 1;
		}
		return 0;
	}

	ModSound::ModSound(const char *filename) : AudioSound()
	{
		mPos = 0;

		FILE* file = fopen(filename, "rb");
		if(!file) {
			// LOG(kLogCritical, "Can not find audio file \"" << filename << "\"");
			return;
		}
		dumb_register_stdfiles();
		fseek(file, 0, SEEK_SET);
		dumbfile = dumbfile_open_stdfile(file);
		myduh	= dumb_read_it(dumbfile);

		if(!myduh) {
			dumbfile_close(dumbfile);
			fseek(file, 0, SEEK_SET);
			dumbfile = dumbfile_open_stdfile(file);
			myduh = dumb_read_xm(dumbfile);
		}
		if(!myduh) {
			dumbfile_close(dumbfile);
			fseek(file, 0, SEEK_SET);
			dumbfile = dumbfile_open_stdfile(file);
			myduh = dumb_read_mod(dumbfile);
		}
		if(!myduh) {
			dumbfile_close(dumbfile);
			fseek(file, 0, SEEK_SET);
			dumbfile = dumbfile_open_stdfile(file);
			myduh = dumb_read_s3m(dumbfile);
		}
        fclose(file);

		sr = duh_start_sigrenderer(myduh, 0, kGlobalAudioChannels, 0);
		if (!sr) {
			// LOG(kLogError, "Dumb file rejected");
			dumbfile_close(dumbfile);
			unload_duh(myduh);
			return;
		}

		mChannels 	= kGlobalAudioChannels;
		mSampleRate = kGlobalAudioSampleRate;
		mBits		= 16;
		mSeekable	= true;
		mSize 		= (((double)duh_get_length(myduh)/65536.0)*kGlobalAudioSampleRate)*4;

		delta 		= 65536.0f/kGlobalAudioSampleRate;

		srit 		= duh_get_it_sigrenderer(sr);
		sd			= duh_get_it_sigdata(myduh);

		dumb_it_set_loop_callback(srit, loop_callback, this);
	}

	ModSound::~ModSound()
	{
		stop();
		duh_end_sigrenderer(sr);
		unload_duh(myduh);
		dumbfile_close(dumbfile);
	}

	int ModSound::fillBuffer(void *buffer, int length)
	{
		if(length == 0 || mStatus == 0)
			return 0;

//		short *buf = (short*)buffer;
		long got = duh_render(sr, 16, 0, 1.0f, delta, length/4, buffer)*4;
		mPos += got;
		return (int)got;
	}

	int ModSound::seek(int pos, AudioMode mode)
	{
		int position = 0;

		if(mode == kAudioModeSequence)
        {
			sr = dumb_it_start_at_order(myduh, kGlobalAudioChannels, pos);
			srit = duh_get_it_sigrenderer(sr);
			dumb_it_set_loop_callback(srit, loop_callback, this);
			mPos = (65536.0f / kGlobalAudioSampleRate) * duh_sigrenderer_get_position(sr);
			return mPos;
		}

		switch(mode)
        {
			case kAudioModeBytes:
				position = pos / (mBits / 8) / mChannels;
				break;
			case kAudioModeSamples:
				position = pos;
				break;
			case kAudioModeMilliseconds:
				position = pos * (mSampleRate / 1000.0);
				break;
			default:
				return mPos;
		}

		sr = duh_start_sigrenderer(myduh, 0, kGlobalAudioChannels, (65536.0f/kGlobalAudioSampleRate) * position );
		srit = duh_get_it_sigrenderer(sr);
		dumb_it_set_loop_callback(srit, loop_callback, this);

		mPos = position * ((mBits / 8) * mChannels);
		return mPos;
	}

	int ModSound::getPosition(AudioMode mode)
	{
		switch (mode)
        {
			case kAudioModeSequence:
				return dumb_it_sr_get_current_order(srit);
			case kAudioModeLine:
				return dumb_it_sr_get_current_row(srit);
            default:
                return AudioSound::getPosition(mode);
		}
	}

	int ModSound::getLength(AudioMode mode)
	{
		switch (mode)
        {
			case kAudioModeSequence:
				return dumb_it_sd_get_n_orders(sd);
            default:
                return AudioSound::getLength(mode);
		}
	}

}
