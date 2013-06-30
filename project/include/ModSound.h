/*
 *  ModPlayer.h
 *
 *  Created by Matthew Tuttle on 7/12/10.
 *  Copyright 2010 Frozen Orange, Inc. All rights reserved.
 *
 */

#ifndef __MODPLAYER_H__
#define __MODPLAYER_H__

#include "AudioSound.h"

class DUH;
class DUMBFILE;
class DUH_SIGRENDERER;
class DUMB_IT_SIGRENDERER;
class DUMB_IT_SIGDATA;

namespace Audaxe
{

	class ModSound : public AudioSound
	{
	public:
		ModSound(const char *filename);
		virtual ~ModSound();

		int fillBuffer(void *buffer, int length);
		int seek(int pos, AudioMode mode);
		int getLength(AudioMode mode);
		int getPosition(AudioMode mode);

	private:
		float delta;

		DUH* myduh;
		DUMBFILE* dumbfile;
		DUH_SIGRENDERER *sr;
		DUMB_IT_SIGRENDERER *srit;
		DUMB_IT_SIGDATA *sd;
	};

}

#endif // __MODPLAYER_H__
