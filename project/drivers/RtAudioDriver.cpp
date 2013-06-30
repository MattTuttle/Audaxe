/*
 *  RtAudioDriver.cpp
 *
 *  Created by Matthew Tuttle on 9/14/09.
 *  Copyright 2009 Frozen Orange. All rights reserved.
 *
 */

#include "RtAudioDriver.h"
#include "Audaxe.h"
#include "AudioEngine.h"

namespace Audaxe
{

	int tick(void *outputBuffer, void *inputBuffer, U32 nBufferFrames,
			double streamTime, RtAudioStreamStatus status, void *userData)
	{
		if (status == 0)
        {
        	AudioEngine *engine = reinterpret_cast<AudioEngine *>(userData);
			engine->processChannels(outputBuffer, nBufferFrames * 8);
        }
		return 0;
	}

	AudioDriver::AudioDriver()
	{
		mTerminated = mActive = false;
		U32 devices = dac.getDeviceCount();
		if (devices < 1)
        {
			// LOG(kLogError, "Audio device not found!");
			return;
		}

		outParams.deviceId = dac.getDefaultOutputDevice();
		outParams.nChannels = 2;
		outParams.firstChannel = 0;
		bufferFrames = kGlobalAudioBufferFrames;
	}

	AudioDriver::~AudioDriver()
	{
		if (!mTerminated)
        {
			terminate();
        }
	}

	int AudioDriver::terminate()
	{
		mTerminated = true;
		dac.closeStream();
		return 0;
	}

	int AudioDriver::init(AudioEngine *engine)
	{
		if (mActive)
        {
			return 1;
        }

		options.flags = RTAUDIO_SCHEDULE_REALTIME;
		options.numberOfBuffers = kGlobalAudioBuffers;
		options.priority = 1;

		try {
			dac.openStream(&outParams, NULL, RTAUDIO_FLOAT32,
						   kGlobalAudioSampleRate, &bufferFrames, &tick, engine, &options);
			dac.startStream();
		} catch (RtError &error) {
			error.printMessage();
		}

		mActive = true;
        return 0;
	}

}
