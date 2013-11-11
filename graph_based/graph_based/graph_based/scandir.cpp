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

#if !defined(IDENT_OFF)
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)
static const char ident[] =
"@(#)$Id: scandir.cpp,v 1.1 2006/02/16 01:23:52 weesan Exp $";
#endif
#endif

#include "scandir.h"

#ifdef _WIN32

// Empty

#elif defined(HAVE_SCANDIR)

// Static members

static int isSelected(const struct dirent *pzcDirent) {
    if (pzcDirent->d_name[0] == '.')
	// The case of . or ..
		return 0;
    else if (strcmp(pzcDirent->d_name, "CVS") == 0)
		return 0;
    else
		return 1;
}

const char *ScanDir::first(void) {
    iCurrentFile = 0;
    if ((iTotalFile = scandir(pzcDir, &pzcNameList, isSelected, 
			      alphasort)) <= 0)
		return NULL;
    else 
		return next();
}

#else

// Empty

#endif // !_WIN32
