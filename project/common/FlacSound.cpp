//
//  FlacSound.cpp
//
//  Created by Matt Tuttle on 6/8/13.
//  Copyright (c) 2013 Matt Tuttle. All rights reserved.
//

#include "FlacSound.h"

namespace Audaxe
{

    FlacSound::FlacSound(const char *filename)
    {
    }

    FlacSound::~FlacSound()
    {
    }

    int FlacSound::fillBuffer(void *inBuffer, int length)
    {
    	return 0;
    }

    int FlacSound::seek(int pos, AudioMode mode)
    {
        if (pos < 0) pos = 0;
        mPos = convertUnitByMode(pos, mode);
        if (mPos > mSize) mPos = mSize;
        return mPos;
    }

}
