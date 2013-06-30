/*
 *  AudioEngine.cpp
 *
 *  Created by Matthew Tuttle on 7/12/10.
 *  Copyright 2010 Frozen Orange, Inc. All rights reserved.
 *
 */

#include <cstdlib>
#include "AudioEngine.h"

#ifdef IPHONE
#include "iOSAudioDriver.h"
#elif ANDROID
#include "OpenSLAudioDriver.h"
#else
#include "RtAudioDriver.h"
#endif

namespace Audaxe
{

    float AudioEngine::volume = 1.0f;

	AudioEngine::AudioEngine()
    {
		driver = 0;
        mNumChannels = 0;
        init();
	}

	AudioEngine::~AudioEngine()
    {
		if(driver)
        {
			driver->terminate();
			delete driver;
		}
	}

	void AudioEngine::init()
    {
		if(driver) return;
		driver = new AudioDriver();
		driver->init(this);
	}

    AudioChannel *AudioEngine::createChannel()
    {
    	if (mNumChannels >= kGlobalMaxChannels) return 0;

        AudioChannel *channel = new AudioChannel();
        mChannelList[mNumChannels++] = channel;
        return channel;
    }

	void AudioEngine::processChannels(void *outBuffer, U32 nBufferFrames)
    {
		memset(outBuffer, 0, nBufferFrames);

        if (mNumChannels < 1) return;

		for (int i = 0; i < mNumChannels; i++)
        {
			mChannelList[i]->fill(outBuffer, nBufferFrames);
		}

		// apply volume & clipping
#if kAudioBits == 16
		short* buf = (short*)outBuffer;
		for(int i = 0; i < (nBufferFrames / (kAudioBits / 8)); i++) {
			*buf *= AudioEngine::volume;
			if(*buf > 32766) {
				*buf = 32766;
			} else if(*buf < -32767) {
				*buf = -32767;
			}
			buf++;
		}
#else
		float* buf = (float*)outBuffer;
		for(int i = 0; i < (nBufferFrames / (kAudioBits / 8)); i++) {
			*buf *= AudioEngine::volume;
			if(*buf > 1.0f) {
				*buf = 1.0f;
			} else if(*buf < -1.0f) {
				*buf = -1.0f;
			}
			buf++;
		}
#endif
	}

}
