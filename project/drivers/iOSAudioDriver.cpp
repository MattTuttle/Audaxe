//
//  iOSAudioDriver.cpp
//
//  Created by Matt Tuttle on 5/31/13.
//  Copyright (c) 2013 Matt Tuttle. All rights reserved.
//

#include <iostream>
#include "iOSAudioDriver.h"
#include "AudioEngine.h"

namespace Audaxe
{

    // passes buffer callback to driver
    void tick(void *userData, AudioQueueRef inAQ, AudioQueueBufferRef outBuffer)
    {
        AudioDriver *pDriver = reinterpret_cast<AudioDriver *>(userData);
        pDriver->bufferCallback(inAQ, outBuffer);
    }

    AudioDriver::AudioDriver()
    {
        mActive = false;

        for (int i = 0; i < kGlobalAudioBuffers; i++)
        {
            mBuffers[i] = NULL;
        }
    }

    AudioDriver::~AudioDriver()
    {

    }

    void AudioDriver::bufferCallback(AudioQueueRef inAQ, AudioQueueBufferRef outBuffer)
    {
        outBuffer->mAudioDataByteSize = mBufferByteSize;
        mEngine->processChannels(outBuffer->mAudioData, mBufferByteSize);

        OSStatus result = AudioQueueEnqueueBuffer(mQueue, outBuffer, 0, NULL);
        if (result) std::cout << "ERROR: " << result << std::endl;
    }

    int AudioDriver::terminate()
    {
        //AudioQueueStop(queue, 0);
        mEngine = 0;
        return 0;
    }

    int AudioDriver::init(AudioEngine *engine)
    {
        if (mActive) return 1;

        mDataFormat.mSampleRate = kGlobalAudioSampleRate;
        mDataFormat.mFormatID = kAudioFormatLinearPCM;
        mDataFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
        mDataFormat.mFramesPerPacket = 1;
        mDataFormat.mChannelsPerFrame = kGlobalAudioChannels; // usually 2 for stereo
        mDataFormat.mBitsPerChannel = kAudioBits;
        mDataFormat.mBytesPerPacket = mDataFormat.mBytesPerFrame = (mDataFormat.mBitsPerChannel / 8) * mDataFormat.mChannelsPerFrame;
        mDataFormat.mReserved = 0;

        OSStatus result = AudioQueueNewOutput(&mDataFormat, tick, this, NULL, NULL, 0, &mQueue);
        if (result) std::cout << "AudioQueueNewOutput: " << result << std::endl;

        mBufferByteSize = kGlobalAudioBufferFrames * mDataFormat.mBytesPerFrame;
        mEngine = engine;

        // prime data before running
        for (int i = 0; i < kGlobalAudioBuffers; ++i) {
            result = AudioQueueAllocateBuffer(mQueue, mBufferByteSize, &mBuffers[i]);
            if (result) std::cout << "AudioQueueAllocateBuffer: " << result << std::endl;
            bufferCallback(mQueue, mBuffers[i]);
        }

        result = AudioQueueSetParameter(mQueue, kAudioQueueParam_Volume, 1.0);
        if (result) std::cout << "AudioQueueSetParameter: " << result << std::endl;

        result = AudioQueueStart(mQueue, NULL);
        if (result) std::cout << "AudioQueueStart: " << result << std::endl;

        mActive = true;
        return 0;
    }

}
