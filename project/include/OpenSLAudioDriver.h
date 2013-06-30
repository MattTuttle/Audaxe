//
//  AndroidAudioDriver.h
//
//  Created by Matt Tuttle on 5/31/13.
//  Copyright (c) 2013 Matt Tuttle. All rights reserved.
//

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

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

        AudioEngine*                  mEngine;
    private:
        void initEngine();

        SLObjectItf                   engineObject;
        SLEngineItf                   engineEngine;
        SLObjectItf                   outputMixObject;
        SLObjectItf                   bqPlayerObject;
        SLPlayItf                     bqPlayerPlay;
        SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;

        bool                          mActive;
    };

}
