// $Id: debug.h,v 1.3 2006/11/03 07:14:43 weesan Exp $

/*
 * debug.h - Simple debug facilities
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	5/16/2005
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef _WIN32
#include <stdio.h>
#define snprintf	_snprintf
#define vsnprintf	_vsnprintf
#endif

#ifdef DEBUG
#undef DEBUG
#endif

void DEBUG(const char *pzcFormat, ...);

#endif // DEBUG_H