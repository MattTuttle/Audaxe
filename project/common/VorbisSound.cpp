//
//  VorbisSound.cpp
//
//  Created by Matt Tuttle on 6/8/13.
//  Copyright (c) 2013 Matt Tuttle. All rights reserved.
//

#include <vorbis/vorbisfile.h>
#include "VorbisSound.h"

#define BUFFER_SIZE 0xFFFF

namespace Audaxe
{

    VorbisSound::VorbisSound(const char *filename)
    {
        int endian = 0; // 0 = little; 1 = big
        int bitStream;
        long bytes;
        char array[BUFFER_SIZE];
        vorbis_info *pInfo;
        OggVorbis_File oggFile;
        FILE *f = fopen(filename, "rb");
        if (!f)
        {
            printf("Could not open %s", filename);
            return;
        }
        ov_open(f, &oggFile, NULL, 0);
        pInfo = ov_info(&oggFile, -1);

        mChannels 	= pInfo->channels;
		mSampleRate = (int)pInfo->rate;
		mBits		= 16;
		mSeekable	= true;

        do {
            // Read up to a buffer's worth of decoded sound data
            bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
            buffer.insert(buffer.end(), array, array + bytes);
        } while (bytes > 0);
        mSize = (int)buffer.size();

        ov_clear(&oggFile);
    }

    VorbisSound::~VorbisSound()
    {
        stop();
        buffer.clear();
    }

    int VorbisSound::fillBuffer(void *inBuffer, int length)
    {
        if(length == 0 || mStatus != kAudioStatusPlaying)
			return 0;

        char *outBuffer = (char *)inBuffer;
        for (int i = 0; i < length; i++)
        {
            // hit end of buffer
            if (mPos >= mSize)
            {
                if (mLoop)
                {
                    mPos = (mLoopStart > 0) ? mLoopStart : 0;
                }
                else
                {
                    setStatus(kAudioStatusStopped);
                    return i;
                }
            }
            else if (mLoop)
            {
                if (mLoopEnd > 0 && mPos >= mLoopEnd)
                {
                    mPos = (mLoopStart > 0) ? mLoopStart : 0;
                }
            }
            *outBuffer++ = buffer[mPos++];
        }
		return length;
    }

    int VorbisSound::seek(int pos, AudioMode mode)
    {
        if (pos < 0) pos = 0;
        mPos = convertUnitByMode(pos, mode);
        if (mPos > mSize) mPos = mSize;
        return mPos;
    }

}
