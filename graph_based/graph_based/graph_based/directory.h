// $Id: directory.h,v 1.1 2005/08/20 18:31:13 weesan Exp $

/*
 * directory.h - Directory utility.
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	8/14/2005
 */

#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stdlib.h>

class Directory {
private:
	static char pzcCwd[_MAX_PATH];

public:
	static const char *cwd(void);
	static bool create(const char *pzcDir, const char *pzcBaseDir = NULL);
};

#endif // DIRECTIORY_H