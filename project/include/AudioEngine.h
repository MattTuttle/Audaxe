/*
 *  AudioEngine.h
 *
 *  Created by Matthew Tuttle on 7/12/10.
 *  Copyright 2010 Frozen Orange, Inc. All rights reserved.
 *
 */

#ifndef __AUDIOENGINE_H__
#define __AUDIOENGINE_H__

#include <vector>

#include "AudioChannel.h"
#include "DataTypes.h"

namespace Audaxe
{

	class AudioDriver;

	class AudioEngine
	{
	public:
		AudioEngine();
		virtual ~AudioEngine();

		void init();

        AudioChannel *createChannel();
		void removeChannel(AudioChannel *channel);
		size_t numChannels() { return mNumChannels; }

		void processChannels(void *outBuffer, U32 nBufferFrames);

        static float volume;

	private:
		AudioDriver *driver;

		AudioChannel *mChannelList[kGlobalMaxChannels];
        U32 mNumChannels;
	};

}

#endif // __AUDIOENGINE_H__
