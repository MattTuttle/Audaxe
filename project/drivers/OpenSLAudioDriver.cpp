//
//  AndroidAudioDriver.cpp
//
//  Created by Matt Tuttle on 5/31/13.
//  Copyright (c) 2013 Matt Tuttle. All rights reserved.
//

#include <android/log.h>
#include <cstdlib>

#include "OpenSLAudioDriver.h"
#include "AudioEngine.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

#define kBufferSize (kGlobalAudioBufferFrames * (kAudioBits / 8) * kGlobalAudioChannels)
int nextBuffer = 0;
char buffers[kGlobalAudioBuffers * kBufferSize];

namespace Audaxe
{

	void tick(SLAndroidSimpleBufferQueueItf bq, void *context)
	{
		SLresult result;
		AudioDriver *driver = reinterpret_cast<AudioDriver *>(context);
		char *pBuffer = &buffers[nextBuffer * kBufferSize];
		LOGI("buffer %d", pBuffer);
		driver->mEngine->processChannels(pBuffer, kBufferSize);
		result = (*bq)->Enqueue(bq, pBuffer, kBufferSize);

		if (++nextBuffer >= kGlobalAudioBuffers) {
			nextBuffer = 0;
		}
	}

	AudioDriver::AudioDriver()
	{
		mActive = false;
		engineObject = NULL;
		outputMixObject = NULL;
		bqPlayerObject = NULL;
		bqPlayerPlay = NULL;
		bqPlayerBufferQueue = NULL;
	}

	AudioDriver::~AudioDriver()
	{
	}

	int AudioDriver::terminate()
	{
		//AudioQueueStop(queue, 0);
		mEngine = 0;
		return 0;
	}

	void AudioDriver::initEngine()
	{
		SLresult result;
		result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
		result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
		result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);

		const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
		const SLboolean req[1] = {SL_BOOLEAN_FALSE};
		result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);

		// realize the output mix
		result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
	}

	int AudioDriver::init(AudioEngine *engine)
	{
		SLresult result;
		if (mActive) return 1;

		mEngine = engine;
		LOGW("Initializing audio driver");

		initEngine();

		// configure audio source
		SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, kGlobalAudioBuffers};
		SLDataFormat_PCM pcm;
		pcm.formatType = SL_DATAFORMAT_PCM;
		pcm.numChannels = kGlobalAudioChannels;
		pcm.samplesPerSec = kGlobalAudioSampleRate * 1000;
		pcm.bitsPerSample = kAudioBits;
		pcm.containerSize = kAudioBits;
		pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
		pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;
		SLDataSource audioSrc = {&loc_bufq, &pcm};

		// configure audio sink
		SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
		SLDataSink audioSnk = {&loc_outmix, NULL};

		// create audio player
		const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
		const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
		result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk, 3, ids, req);
		if (SL_RESULT_SUCCESS != result) LOGW("ERROR: Could not create audio player");

		result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
		if (SL_RESULT_SUCCESS != result) LOGW("ERROR: Could not realize player object");

		// get the play interface
		result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
		if (SL_RESULT_SUCCESS != result) LOGW("ERROR: Could not get play interface");

		// get the buffer queue interface
		result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue);
		if (SL_RESULT_SUCCESS != result) LOGW("ERROR: Could not get buffer interface");

		result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, tick, this);
		if (SL_RESULT_SUCCESS != result) LOGW("ERROR: Could not get buffer queue");

		result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
		if (SL_RESULT_SUCCESS != result) LOGW("ERROR: Could not set play state");

		for (int i = 0; i < kGlobalAudioBuffers; ++i) {
			tick(bqPlayerBufferQueue, this);
		}

		mActive = true;
		return 0;
	}

}
