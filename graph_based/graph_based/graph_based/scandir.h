/*

 This library is copyrighted(C) 1999 by WeeSan Lee <weesan@employees.org>, 
 with all rights reserved.  

 This program is free software;  please feel free to redistribute it 
 and/or modify it under the terms of the GNU General Public License 
 (version 2) as published by the Free Software Foundation.  It is 
 distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 PARTICULAR PURPOSE.  See the GNU General Public License for more details.

 */

// $Id: scandir.h,v 1.1 2006/02/16 01:23:52 weesan Exp $

#ifndef SCANDIR_H
#define SCANDIR_H

#include <string.h>

#ifdef _WIN32

#include <stdio.h>
#include <windows.h>

class ScanDir : public WIN32_FIND_DATA {
private:
    HANDLE hFind;
    char pzcDir[256], pzcFilter[32];
	char pzcPath[256];

public: 
    ScanDir(const char *_pzcDir, const char *_pzcFilter = NULL) :
		hFind(INVALID_HANDLE_VALUE) {
		pzcDir[0] = pzcFilter[0] = pzcPath[0] = 0;
		if (_pzcDir) {
			_snprintf(pzcDir, sizeof(pzcDir), "%s", _pzcDir);
		}
		if (_pzcFilter) {
			_snprintf(pzcFilter, sizeof(pzcFilter), "%s", _pzcFilter);
		}
    }
	~ScanDir(void) {
		if (hFind != INVALID_HANDLE_VALUE) {
			FindClose(hFind);
		}
	}
	const char *dir(void) const {
		return (pzcDir);
	}
	const char *filter(void) const {
		if (pzcFilter[0]) {
			return (pzcFilter);
		} else {
			return (NULL);
		}
	}
	const char *path(void) {
		_snprintf(pzcPath, sizeof(pzcPath), "%s/%s", dir(), filename());
		return (pzcPath);
	}
	const char *filename(void) const {
		return (cFileName);
	}
    const char *first(void) {
		char pzcPattern[256];
		if (filter()) {
			_snprintf(pzcPattern, sizeof(pzcPattern),
					  "%s/%s", dir(), filter());
		} else {
			_snprintf(pzcPattern, sizeof(pzcPattern),
					  "%s", dir());
		}
		if ((hFind = FindFirstFile(pzcPattern, this)) != INVALID_HANDLE_VALUE) {
			return (filename());
		} else {
			return NULL;
		}
    }
    const char *next(void) {
		if (FindNextFile(hFind, this)) {
			return (filename());
		} else {
			return NULL;
		}
    }
};

#elif HAVE_SCANDIR

#include <stdio.h>
#include <dirent.h>

class ScanDir {
private:
    struct dirent **pzcNameList;
    int iTotalFile, iCurrentFile;
    char pzcDir[256], pzcFilename[512];

public:
    ScanDir(const char *_pzcDir) : iTotalFile(0), iCurrentFile(0) {
		strcpy(pzcDir, _pzcDir);
		pzcFilename[0] = 0;
    }
    const char *first(void);
    const char *next(void) {
	if (iCurrentFile < iTotalFile) {
	    sprintf(pzcFilename, "%s/%s", 
			    pzcDir, pzcNameList[iCurrentFile++]->d_name);
	    return pzcFilename;
	} else
	    return NULL;
    }
};

#else

#include <stdio.h>
#include <dirent.h>

class ScanDir {
private:
    char pzcDir[256], pzcFilename[512];
    DIR *psDir;

public:
    ScanDir(const char *_pzcDir) : 
		psDir(NULL) {
		pzcFilename[0] = 0;
		strcpy(pzcDir, _pzcDir);
		psDir = opendir(pzcDir);
    }
    const char *first(void) {
	if (psDir == NULL) return NULL;
	while (1) {
	    struct dirent *psDirent = readdir(psDir);
	    if (psDirent == NULL) return NULL;
	    if (psDirent->d_name[0] == '.') continue;
	    sprintf(pzcFilename, "%s/%s",
			    pzcDir, psDirent->d_name);
	    return pzcFilename;
	}
	// Shouldn't reach here!
	return NULL;
    }
    const char *next(void) {
		return first();
    }
};

#endif // _WIN32

#endif // SCANDIR_H
