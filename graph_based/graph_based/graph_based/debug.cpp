// $Id: debug.cpp,v 1.2 2006/11/03 07:14:43 weesan Exp $

/*
 * debug.cpp - Simple debug facilities
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	5/16/2005
 */

#include <stdio.h>
#include <stdarg.h>
#include "stdafx.h"
#include "debug.h"

#define DEBUG_GUI

void DEBUG(const char *pzcFormat, ...) {
	va_list ap;
	char pzcBuffer[256];

	va_start(ap, pzcFormat);
	vsnprintf(pzcBuffer, sizeof(pzcBuffer), pzcFormat, ap);
#ifdef DEBUG_GUI
	AfxMessageBox(pzcBuffer);
#else
	printf(pzcBuffer);
#endif
	va_end(ap);
}
