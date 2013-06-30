//
//  VorbisSound.h
//
//  Created by Matt Tuttle on 6/8/13.
//  Copyright (c) 2013 Matt Tuttle. All rights reserved.
//

#ifndef __VORBISSOUND_H__
#define __VORBISSOUND_H__

#include <vector>
#include "AudioSound.h"

namespace Audaxe
{

	class VorbisSound : public AudioSound
	{
	public:
		VorbisSound(const char *filename);
		virtual ~VorbisSound();

		int fillBuffer(void *buffer, int length);
		int seek(int pos, AudioMode mode);
		int getLength(AudioMode mode);
		int getPosition(AudioMode mode);

	private:
        std::vector<char> buffer;
	};

}

#endif // __VORBISSOUND_H__
