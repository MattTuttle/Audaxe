/*
 *  AudioPlayer.h
 *
 *  Created by Matthew Tuttle on 7/2/10.
 *  Copyright 2010 Frozen Orange, Inc. All rights reserved.
 *
 */

#ifndef __AUDIOPLAYER_H__
#define __AUDIOPLAYER_H__

#include "DataTypes.h"
#include "Audaxe.h"

namespace Audaxe
{

	class AudioSound
	{
	public:
		AudioSound()
		{
			mSampleRate = 0;
			mChannels = 0;
			mFrames = 0;
			mBits = 0;
			mStatus = kAudioStatusStopped;
			mSize = 0;
			mMixRate = 0;

			mLoop = false;
			mLoopStart = mLoopEnd = -1;

			mSeekable = false;
			mReversable = false;
		}

        virtual ~AudioSound() { }

		virtual int fillBuffer(void *buffer, int length) = 0;
		virtual int seek(int pos, AudioMode mode) = 0;

		virtual void stop() {
			mPos = 0;
			setStatus(kAudioStatusStopped);
		}

		virtual void play() {
			stop();
			resume();
		}

		virtual void resume() {
			setStatus(kAudioStatusPlaying);
		}

		virtual void pause() {
			setStatus(kAudioStatusStopped);
		}

		bool isPlaying() {
			return mStatus == kAudioStatusPlaying;
		}

		int getLength(AudioMode mode) {
			return convertUnitByMode(mSize, mode);
		}

		int getPosition(AudioMode mode) {
			return convertUnitByMode(mPos, mode);
		}

        int inline convertUnitByMode(int value, AudioMode mode)
        {
            switch (mode) {
				case kAudioModeBytes:
					return value;
				case kAudioModeSamples:
					return value * ((mBits / 8) * mChannels);
				case kAudioModeMilliseconds:
					return value * ((mBits / 8) * mChannels) * (mSampleRate / 1000);
                default:
                    // not supported
                    return 0;
			}
        }

        void setLoopPoints(int start, int end, AudioMode mode)
        {
            if (start > 0)
                mLoopStart = convertUnitByMode(start, mode);
            if (end > 0)
                mLoopEnd = convertUnitByMode(end, mode);
        }

		int  getStatus() { return mStatus; }
		void setStatus(int state) { mStatus = state; }
		int  getNumChannels() { return mChannels; }
		int  getBits() { return mBits; }
		int  getSampleRate() { return mSampleRate; }
		void setRate(double rate) { mMixRate = rate; }
		bool getLoopMode() { return mLoop; }
		void setLoopMode(bool mode) { mLoop = mode; }
		bool isSeekable() { return mSeekable; }

	protected:
		int mSampleRate;
		int mChannels;
		int mFrames;
		int mBits;
		int mSize;
		bool mSeekable;
		int mPos;
		double mMixRate;
		bool mReversable;

		bool mLoop;
		int mLoopStart;
		int mLoopEnd;

		int mStatus;
	};

}

#endif // __AUDIOPLAYER_H__
