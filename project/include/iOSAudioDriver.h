//
//  AQPlayer.h
//
//  Created by Matt Tuttle on 5/31/13.
//  Copyright (c) 2013 Matt Tuttle. All rights reserved.
//

#include <AudioToolbox/AudioToolbox.h>
#include "Audaxe.h"

namespace Audaxe
{

    class AudioEngine;

    class AudioDriver
    {
    public:
        AudioDriver();
        ~AudioDriver();

        virtual int init(AudioEngine *engine);
        virtual int terminate();

        void bufferCallback(AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);

    private:
        AudioStreamBasicDescription   mDataFormat;
        AudioQueueRef                 mQueue;
        AudioQueueBufferRef           mBuffers[kGlobalAudioBuffers];
        AudioFileID                   mAudioFile;
        UInt32                        mBufferByteSize;
        SInt64                        mCurrentPacket;
        UInt32                        mNumPacketsToRead;
        AudioStreamPacketDescription* mPacketDescs;
        AudioEngine*                  mEngine;
        bool                          mActive;
    };

}
