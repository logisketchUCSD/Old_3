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
"@(#)$Id: thread.cpp,v 1.1 2005/08/20 18:31:13 weesan Exp $";
#endif
#endif

#include "thread.h"
#include <stdio.h>

// Static thread call back function
#ifdef _WIN32
void 
#else
void *
#endif
__threadCallBackFunc(void *pvArgv) {
    Thread *pcThread = (Thread *)pvArgv;
    // Should the object returns OK, proc() will be executed
    if (pcThread->ok()) {
	pcThread->proc();
    }
    // Give object a chance to clean up itself before ends
    pcThread->cleanUp();
    // Delete the object before returning/ending the thread
    delete pcThread;
#ifndef _WIN32
    return NULL;
#endif
}

//////////////////////

#ifdef _WIN32

void Thread::start(void) {
    _beginthread(__threadCallBackFunc, 0, this);
}

#else

void Thread::start(void) {
    pthread_attr_t tThreadAttr;
    pthread_attr_init(&tThreadAttr);
    pthread_attr_setdetachstate(&tThreadAttr, eType);
    pthread_create(&tThreadID, 
		   &tThreadAttr, 
		   __threadCallBackFunc, 
		   this);
    // According to man page, the following function call 
    // does nothing in LinuxThreads implementation
    pthread_attr_destroy(&tThreadAttr);
}

#endif
