/*
 *  RtAudioDriver.h
 *
 *  Created by Matthew Tuttle on 9/14/09.
 *  Copyright 2009 Frozen Orange. All rights reserved.
 *
 */

#ifndef __RTAUDIODRIVER_H__
#define __RTAUDIODRIVER_H__

#include "RtAudio.h"
#include "DataTypes.h"

namespace Audaxe
{

    class AudioEngine;

	class AudioDriver {
	public:
		AudioDriver();
		~AudioDriver();

		virtual int init(AudioEngine *engine);
		virtual int terminate();

	private:
		bool mActive, mTerminated;
		U32 bufferFrames;

		RtAudio dac;
		RtAudio::StreamParameters outParams;
		RtAudio::StreamOptions options;
	};

}

#endif // __RTAUDIODRIVER_H__
