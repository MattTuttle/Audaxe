/*
 *  Resampler.h
 *
 *  Created by Matthew Tuttle on 7/12/10.
 *  Copyright 2010 Frozen Orange, Inc. All rights reserved.
 *
 */

#ifndef __RESAMPLER_H__
#define __RESAMPLER_H__

namespace Audaxe
{

	class Resampler
	{
	public:

		int    mStatus;

		int    inBufferSize;
		char*  inBuffer;
		int    inStart;
		int    inSamples;
		int    inChannels;
		int    inBits;
		int    inCharsPerSample;
		double inSampleRate;
		double inCursor;

		int    outBufferSize;
		void*  outBuffer;
		int    outSamples;
		int    outComplete;
		int    outChannels;
		int    outBits;
		int    outCharsPerSample;
		double outSampleRate;
		double outRate;
		double outVolume;
		double outPanL;
		double outPanR;
		int    outCursor;
		double outl;
		double outr;
		double outLevelLeft;
		double outLevelRight;
		int    outRequestSize;
		double outRateAdjuster;
		int    outAdjusterCount;
		int    outAdjusterTick;
		double outAdjusterRealRate;

		Resampler(int channels,int bits,int rate);
		~Resampler();

		void setInputFormat(int channels, int bits, int rate);
		void requestOutput(void* buffer,int size);
		void pushInput(int size);

		void setPan(double pan);

		int process();

		// -------------------------------------------------------------------------

		int outMono8() { return 0; }
		int outMono16() { return 0; }
		int outMono32() { return 0; }
		int outStereo8() { return 0; }
		int outStereo16();
		int outStereo32();
		double interpolate(double y1,double y2,double mu);
		double interpolateU8(double y1,double y2,double mu);
	};

}

#endif // __RESAMPLER_H__
