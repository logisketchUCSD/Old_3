// $Id: directory.cpp,v 1.1 2005/08/20 18:31:13 weesan Exp $

/*
 * directory.cpp - Directory utility.
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	8/14/2005
 */

#include "stdafx.h"
#include "directory.h"
#include "debug.h"

char Directory::pzcCwd[_MAX_PATH];

const char *Directory::cwd(void) {
	if (pzcCwd[0]) {
		return (pzcCwd);
	} else {
		DWORD ret = GetCurrentDirectory(sizeof(pzcCwd), pzcCwd);
		if (ret == 0) {
			return (NULL);
		} else {
			return (pzcCwd);
		}
	}
}

bool Directory::create(const char *pzcDir, const char *pzcBaseDir) {
	char pzcBuffer[_MAX_PATH];
	if (pzcBaseDir) {
		snprintf(pzcBuffer, sizeof(pzcBuffer), "%s\\%s", pzcBaseDir, pzcDir);
	} else {
		snprintf(pzcBuffer, sizeof(pzcBuffer), "%s", pzcDir);
	}

	if (CreateDirectory(pzcBuffer, NULL)) {
		return (true);
	} else {
		return (false);
	}
}
