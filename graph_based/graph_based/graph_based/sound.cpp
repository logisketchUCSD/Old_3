// $Id: sound.cpp,v 1.1 2006/02/10 22:12:47 weesan Exp $

/*
 * sound.cpp - A sound class to play digits and alphabets in the future
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	1/17/2006
 */

#include <stdafx.h>
#include <mmsystem.h>
#include <string.h>
#include <assert.h>
#include "sound.h"
#include "debug.h"

Sound::Sound(const char *pzcWords) : 
	pzcWords(strdup(pzcWords)) {
	assert(pzcWords != NULL);
}

void Sound::play(void) {
	const char *delim = " \t\n";
	for (const char *p = strtok(words(), delim); p; p = strtok(NULL, delim)) {
		if (isdigit(*p)) {
			playDigit(p);
		} else if (isalpha(*p)) {
			playAlpha(p);
		} else {
			fprintf(stderr, "Don't know how to sound '%s'\n", p);
		}
	}
}

void Sound::playChar(const char ucChar) {
	char pzcFile[_MAX_PATH];
	snprintf(pzcFile, sizeof(pzcFile), "%s\\%c.wav", SOUND_DIR, ucChar);
	//printf("[%s]\n", pzcFile);
	PlaySound(pzcFile, NULL, SND_APPLICATION | SND_FILENAME | SND_SYNC);
}

void Sound::playWord(const char *pzcWord) {
	char pzcFile[_MAX_PATH];
	snprintf(pzcFile, sizeof(pzcFile), "%s\\%s.wav", SOUND_DIR, pzcWord);
	//printf("[%s]\n", pzcFile);
	PlaySound(pzcFile, NULL, SND_APPLICATION | SND_SYNC);
}

void Sound::playDigit(const char *pzcDigit) {
	for (const char *p = pzcDigit; p && *p; p++) {
		playChar(*p);
	}
}

void Sound::playAlpha(const char *pzcAlpha) {
	switch (*pzcAlpha) {
	case 'i':
	case 'I':
		if (stricmp(pzcAlpha, "idiot") == 0) {
			playWord("idiot");
			return;
		}
		break;
	case 'y':
	case 'Y':
		if (stricmp(pzcAlpha, "you") == 0) {
			playWord("you");
			return;
		}
		break;
	default:
		break;
	}

	for (const char *p = pzcAlpha; p && *p; p++) {
		playChar(*p);
	}
}
