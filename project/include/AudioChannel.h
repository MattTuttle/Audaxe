/*
 *  AudioChannel.h
 *
 *  Created by Matthew Tuttle on 7/12/10.
 *  Copyright 2010 Frozen Orange, Inc. All rights reserved.
 *
 */

#ifndef __AUDIOCHANNEL_H__
#define __AUDIOCHANNEL_H__

#include "AudioSound.h"
#include "DataTypes.h"
#include "Resampler.h"

namespace Audaxe
{

	class AudioChannel
	{
	public:
		AudioChannel();
		~AudioChannel();

		void setSound(AudioSound *sound);

		void stop();
		void setPaused(bool paused);
		void setPan(float pan);
		void setVolume(float volume);
		void setRate(float rate);
		bool playing();
//		U32  getPosition(U32 mode);
//		U32  getLength(U32 mode);
//		U32  seek(U32 position, U32 mode);

//		U32  getLoopMode();
//		void setLoopMode(U32 mode);
//		void setLoopPoints(U32 firstSample, U32 length);

//		void getUV(double *L, double *R);
//		bool isSeekable();
		int fill(void *outBuffer, int nBufferFrames);

	private:
		AudioSound *mAudio;
		Resampler *mResampler;
	};

}

#endif // __AUDIOCHANNEL_H__
