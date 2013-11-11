// $Id: sound.h,v 1.1 2006/02/10 22:12:47 weesan Exp $

/*
 * sound.h - A sound class to play digits and alphabets in the future
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	1/17/2006
 */

#ifndef SOUND_H
#define SOUND_H

#include <stdlib.h>

//#define SOUND_DIR	"sounds"
// XXX
#define SOUND_DIR	"C:\\Documents and Settings\\weesan\\My Documents\\source\\graph_based\\sounds"

class Sound {
private:
	char *pzcWords;

private:
	void playChar(const char ucChar);
	void playWord(const char *pzcWord);
	void playDigit(const char *pzcDigit);
	void playAlpha(const char *pzcAlpha);

public:
	Sound(const char *_pzcWords);
	~Sound(void) {
		if (pzcWords) {
			free(pzcWords);
		}
	}
	char *words(void) const {
		return (pzcWords);
	}
	void play(void);
};

#endif // SOUND_H