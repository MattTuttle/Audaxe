/*
 *  AudioChannel.cpp
 *
 *  Created by Matthew Tuttle on 7/12/10.
 *  Copyright 2010 Frozen Orange, Inc. All rights reserved.
 *
 */

#include <cstring>
#include "AudioChannel.h"

float inline clamp(float value, float min, float max)
{
	return (value < min ? min : (value > max ? max : value));
}

namespace Audaxe
{

	AudioChannel::AudioChannel()
	{
		mAudio = 0;
		mResampler = new Resampler(kGlobalAudioChannels, kAudioBits, kGlobalAudioSampleRate);
	}

	AudioChannel::~AudioChannel()
	{
		if (mAudio) delete mAudio;
		delete mResampler;
	}

	void AudioChannel::setSound(AudioSound *sound)
	{
		if (mAudio)
		{
			if (mAudio->isPlaying()) sound->play();
			delete mAudio;
		}
		mAudio = sound;
		mResampler->setInputFormat(mAudio->getNumChannels(), mAudio->getBits(), mAudio->getSampleRate());
	}

	void AudioChannel::setPan(float pan)
	{
		mResampler->setPan(clamp(pan, -1.0f, 1.0f));
	}

	void AudioChannel::setVolume(float volume)
	{
		mResampler->outVolume = clamp(volume, 0.0f, 1.0f);
	}

	void AudioChannel::setRate(float rate)
	{
		if(rate < 0) return;
		mResampler->outRate = rate;
		if (mAudio) mAudio->setRate(rate);
	}

	int AudioChannel::fill(void *outBuffer, int nBufferFrames)
	{
		if (!mAudio) return 0;
		if (!mAudio->getStatus())
		{
			if(mResampler->outRateAdjuster == 0.0)
				return 0;
		}

		char *inBuf = 0;
		int inSize = 0;

		mResampler->mStatus = mAudio->getStatus();
		mResampler->requestOutput(outBuffer, nBufferFrames);
		while (mResampler->process())
		{
			inBuf = (char*)(mResampler->inBuffer) + mResampler->inStart;
			inSize = mResampler->inBufferSize - mResampler->inStart;
			//  memset(inBuf, 0, inSize); // redundant
			mAudio->setRate(mResampler->outAdjusterRealRate);
			mResampler->pushInput(mAudio->fillBuffer(inBuf, inSize));
		}
		return nBufferFrames;
	}

}
