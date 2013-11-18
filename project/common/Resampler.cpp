/*
 *  Resampler.cpp
 *
 *  Created by Matthew Tuttle on 7/12/10.
 *  Copyright 2010 Frozen Orange, Inc. All rights reserved.
 *
 */

#include <iostream>
#include <cmath>
#include "Resampler.h"
#include "AudioEngine.h"

namespace Audaxe
{

	Resampler::Resampler(int channels,int bits,int rate)
	{
		mStatus           = 0;

		inBufferSize      = 128 * ((bits / 8) * channels);
		inBuffer          = new char[inBufferSize];
		inStart           = 0;
		inSamples         = 0;
		inChannels        = 0;
		inBits            = 0;
		inSampleRate      = 0;
		inCursor          = 0;
		memset(inBuffer, 0, inBufferSize);

		outBufferSize       = 0;
		outBuffer           = NULL;
		outSamples          = 0;
		outComplete         = 0;
		outChannels         = channels;
		outBits             = bits;
		outSampleRate       = rate;
		outRate             = 1.0;
		outVolume           = 1.0;
		outPanL             = 1.0;
		outPanR             = 1.0;
		outCursor           = 0;
		outl                = 0;
		outr                = 0;
		outLevelLeft        = 0;
		outLevelRight       = 0;
		outRequestSize      = 0;
		outRateAdjuster     = 0.0;
		outAdjusterCount    = 0;
		outAdjusterTick     = 512;
		outAdjusterRealRate = 0;

	}

	Resampler::~Resampler(){
		delete[] inBuffer;
	}

	void Resampler::setInputFormat(int channels, int bits, int rate) {
		inChannels   = channels;
		inBits       = bits;
		inSampleRate = rate;
	}

	void Resampler::requestOutput(void* buffer, int size) {
		outBuffer      = buffer;
		outSamples     = size / (outBits / 8) / outChannels;
		outRequestSize = outSamples;
		outComplete    = 0;
		outCursor      = 0;
		outl           = 0;
		outr           = 0;
	}

	void Resampler::pushInput(int size)
	{
		// prevent divide by zero
		if (inBits == 0 || inChannels == 0) return;
		inSamples = (size + inStart) / (inBits / 8) / inChannels;
		if (outAdjusterRealRate < 0)
		{
			inCursor = inSamples - 2;
		}
	}

	void Resampler::setPan(double pan)
	{
		if (pan == 0) { outPanL = 1.0; outPanR = 1.0; }
		else if (pan < 0) { outPanL = 1.0; outPanR = 1.0 - fabs(pan); }
		else { outPanL = 1.0 - pan; outPanR = 1.0; }
	}

	int Resampler::process()
	{
		if (inBufferSize == 0)    return 1;	// more input is required
		if (inCursor > inSamples) return 1;	// more input is required
		if (outComplete == 1)     return 0;	// output is available

		switch(outChannels)
		{
			case 1:
		//			switch(outBits){
		//				case 8:	return outMono8();
		//				case 16:	return outMono16();
		//				case 32:	return outMono32();
		//			}
				break;
			case 2:
				switch(outBits)
				{
		//				case 8:	return outStereo8();
					case 16: return outStereo16();
					case 32: return outStereo32();
				}
				break;
		}

		return -1;	// something is wrong
	}


	int Resampler::outStereo16()
	{
		short* outL = static_cast<short*>(outBuffer); outL += (outCursor * outChannels);
		short* outR = outL + 1;
		double char2short = 32767 / 128;
		double float2short= 32767;
		int    s = (inBits / 8) * inChannels;
		double add = (inSampleRate / outSampleRate) * outRate;
		double frac = 0;
		double av;

		char*  inC;
		short* inS;
		float* inF;

		for (int i = outCursor; i < outSamples; i++)
		{
			frac = inCursor - static_cast<int>(inCursor);

			switch (inBits)
			{
				case 8:
					inC = reinterpret_cast<char*>(inBuffer);
					switch (inChannels)
					{
						case 1:
							inC += (int)inCursor;
							av = (interpolateU8(*inC, *(inC + 1), frac) * outVolume) * char2short;
							*outL += av * outPanL;
							*outR += av * outPanR;
							break;
						case 2:
							inC += ((int)inCursor) * 2;
							*outL += (interpolateU8(*inC, *(inC + 2), frac) * outVolume * outPanL) * char2short;
							*outR += (interpolateU8(*(inC + 1), *(inC + 3), frac) * outVolume * outPanR) * char2short;
							break;
					}
					break;
				case 16:
					inS = reinterpret_cast<short*>(inBuffer);
					switch (inChannels)
					{
						case 1:
							inS += (int)inCursor;
							av = interpolate(*inS, *(inS + 1), frac) * outVolume;
							*outL += av * outPanL;
							*outR += av * outPanR;
							break;
						case 2:
							inS += ((int)inCursor) * 2;
							*outL += interpolate(*inS, *(inS + 2), frac) * outVolume * outPanL;
							*outR += interpolate(*(inS + 1), *(inS + 3), frac) * outVolume * outPanR;
//							*outL += *(inS) * outVolume * outPanL;
//							*outR += *(inS + 1) * outVolume * outPanR;
							break;
					}
					break;
				case 32:
					inF = reinterpret_cast<float*>(inBuffer);
					switch (inChannels)
					{
						case 1:
							inF += (int)inCursor;
							av = (interpolate(*inF, *(inF + 1), frac) * outVolume) * float2short;
							*outL += av * outPanL;
							*outR += av * outPanR;
							break;
						case 2:
							inF += ((int)inCursor) * 2;
							*outL += (interpolate(*inF, *(inF + 2), frac) * outVolume * outPanL) * float2short;
							*outR += (interpolate(*(inF + 1), *(inF + 3), frac) * outVolume * outPanR) * float2short;
							break;
					}
					break;

			}

			// next sample
			outL += 2;
			outR += 2;
			outCursor++;
			inCursor += add;
			if (inCursor + 1 >= inSamples)
			{
				memcpy(inBuffer, (char*)(inBuffer) + (inBufferSize - s), s);
				inStart = s;
				inCursor = inCursor - (int)inCursor;
				return 1;
			}

		} //next

		outComplete = 1;
		return 0;

	}

	int Resampler::outStereo32()
	{
		float* outL = static_cast<float*>(outBuffer); outL += (outCursor * outChannels);
		float* outR = outL + 1;
		int    bytes = inBits / 8;
		int    s = bytes * inChannels;
		double add;
		double frac = 0;
		double av;

		double conversion = inBits;
		unsigned char* inC;
		short* inS;
		float* inF;

		switch (inBits)
		{
			case 32:
				conversion = 1.0;
				break;
			default:
				conversion = 0.5 / pow(2, inBits); // half amplitude
				break;
		}

		if (mStatus == 1)
		{
			add = (inSampleRate / outSampleRate) * (outRate + outRateAdjuster);
		}
		else
		{
			add = (inSampleRate / outSampleRate) * outRateAdjuster;
		}
		outAdjusterRealRate = add;

		for (int i = outCursor; i < outSamples; i++)
		{
			if (inCursor < 0) break;
			frac = inCursor - static_cast<int>(inCursor);

			if (outAdjusterCount == outAdjusterTick)
			{
				outAdjusterCount = 0;
				outRateAdjuster *= 0.9;
				// if(fabs(outRateAdjuster) < 0.001) outRateAdjuster = 0;
				if (mStatus == 1)
				{
					add = (inSampleRate / outSampleRate) * (outRate + outRateAdjuster);
				}
				else
				{
					add = (inSampleRate / outSampleRate) * outRateAdjuster;
				}
				outAdjusterRealRate = add;
			}
			else
			{
				outAdjusterCount++;
			}

			switch (inBits)
			{
				case 8:
					inC = reinterpret_cast<unsigned char*>(inBuffer);
					switch (inChannels)
					{
						case 1:
							inC += (int)inCursor;
							av = (interpolateU8(*inC, *(inC + 1), frac) * outVolume) * conversion;
							*outL += av * outPanL;
							if (fabs(av) * outPanL > outl) outl = fabs(av) * outPanL;
							*outR += av * outPanR;
							if (fabs(av) * outPanR > outr) outr = fabs(av) * outPanR;
							break;
						case 2:
							inC += ((int)inCursor) * 2;
							av = (interpolateU8(*inC, *(inC + 2), frac) * outVolume * outPanL) * conversion;
							*outL += av;
							if (fabs(av) > outl) outl = fabs(av);
							av = (interpolateU8(*(inC + 1), *(inC + 3), frac) * outVolume * outPanR) * conversion;
							*outR += av;
							if (fabs(av) > outr) outr = fabs(av);
							break;
					}
					break;

				case 16:
					inS = reinterpret_cast<short*>(inBuffer);
					switch (inChannels)
					{
						case 1:
							inS += (int)inCursor;
							av = (interpolate(*inS, *(inS + (int)add), frac) * outVolume) * conversion;
							*outL += av * outPanL;
							if (fabs(av) * outPanL > outl) outl = fabs(av) * outPanL;
							*outR += av * outPanR;
							if (fabs(av) * outPanR > outr) outr = fabs(av) * outPanR;
							break;
						case 2:
							inS += ((int)inCursor) * 2;
							av = (interpolate(*inS, *(inS + 2), frac) * outVolume * outPanL) * conversion;
							*outL += av;
							if (fabs(av) > outl) outl = fabs(av);
							av = (interpolate(*(inS + 1), *(inS + 3), frac) * outVolume * outPanR) * conversion;
							*outR += av;
							if (fabs(av) > outr) outr = fabs(av);
							break;
					}
					break;

				case 32:
					inF = reinterpret_cast<float*>(inBuffer);
					switch (inChannels)
					{
						case 1:
							inF += (int)inCursor;
							av = interpolate(*inS, *(inS + (int)add), frac) * outVolume;
							*outL += av * outPanL;
							if (fabs(av) * outPanL > outl) outl = fabs(av) * outPanL;
							*outR += av * outPanR;
							if (fabs(av) * outPanR > outr) outr = fabs(av) * outPanR;
							break;
						case 2:
							inF += ((int)inCursor) * 2;
							av = interpolate(*inF, *(inF + 2), frac) * outVolume * outPanL;
							*outL += av;
							if (fabs(av) > outl) outl = fabs(av);
							av = interpolate(*(inF + 1), *(inF + 3), frac) * outVolume * outPanR;
							*outR += av;
							if (fabs(av) > outr) outr = fabs(av);
							break;
					}
					break;

			}

			outL += inChannels;
			outR += inChannels;
			outCursor++;
			inCursor += add;
			if (add > 0)
			{
				if (inCursor + 1 >= inSamples)
				{
					memcpy(inBuffer, (char*)(inBuffer) + (inBufferSize - s), s);
					inStart = s;
					inCursor = inCursor - (int)inCursor;
					return 1;
				}
			}
			else if (add < 0)
			{
				if (inCursor - 1 < 0)
				{
					return 1;
				}

			}

		} //next

		outl *= AudioEngine::volume;
		if (outl > 1.0) outl = 1.0;
		if (outl > outLevelLeft) {outLevelLeft = outl;}
		else { outLevelLeft += (outl - outLevelLeft) * 0.25; }

		outr *= AudioEngine::volume;
		if (outr > 1.0) outr = 1.0;
		if (outl > outLevelRight) {outLevelRight = outr;}
		else { outLevelRight += (outr - outLevelRight) * 0.25; }

		outComplete = 1;
		return 0;

	}

	double inline Resampler::interpolate(double y1, double y2, double mu)
	{
		mu -= static_cast<int>(mu);
#ifdef IPHONE
		// Linear interpolation
		return y1 + ((y2 - y1) * mu);
#else
		// Cosine interpolation
		double mu2 = (1 - cos(mu * M_PI)) / 2;
		return (y1 * (1 - mu2) + y2 * mu2);
#endif
	}

	double inline Resampler::interpolateU8(double y1, double y2, double mu)
	{
		y1 -= 128;
		y2 -= 128;
		return interpolate(y1, y2, mu);
	}

}
