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

// $Id: thread.h,v 1.1 2005/08/20 18:31:13 weesan Exp $

#ifndef THREAD_H
#define THREAD_H

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <list>
typedef unsigned long pthread_t;
#else
#include <pthread.h>
#include <errno.h>
#include <list>
#endif

using namespace std;

class Thread {
    friend 
#ifdef _WIN32
    void 
#else
    void *
#endif
    __threadCallBackFunc(void *pvArgv);
    
public:
    enum Type {
#ifndef _WIN32
	Joinable = PTHREAD_CREATE_JOINABLE,
	Detached = PTHREAD_CREATE_DETACHED,
#else
	Windows
#endif
    };
    
private:
    pthread_t tThreadID;
    Type eType;
    
protected:
    virtual void proc(void) = 0;
    virtual void cleanUp(void) = 0;

public:
#ifdef _WIN32
    Thread(void) : tThreadID(0), eType(Windows) {}
    virtual ~Thread(void) {}
#else
    Thread(Type eType = Detached) : tThreadID(0), eType(eType) {}
    virtual ~Thread(void) {
	if (eType == Joinable) {
	    pthread_detach(tid());
	    pthread_cancel(tid());
	}
    }
#endif
    pthread_t tid(void) const {
	return tThreadID;
/*
#ifdef _WIN32
      return GetCurrentThreadId();
#else
      return pthread_self();
#endif
*/
    }
    virtual bool ok(void) const = 0;
    void start(void);
    Type type(void) const {
	return eType;
    }
};

// Thread pointers will be deleted after being used automatically!
class Threads : public list<Thread *> {
public:
    void join(void) {
#ifndef _WIN32
	for (iterator itr = begin(); itr != end(); itr++) 
	    if ((*itr)->type() == Thread::Joinable)
		pthread_join((*itr)->tid(), NULL);
#endif
    }
};

#ifdef _WIN32

class Mutex {
private:
    CRITICAL_SECTION cs;
    
public:
    Mutex(void) {
	InitializeCriticalSection(&cs);
    }
    ~Mutex(void) {
	DeleteCriticalSection(&cs);
    }
    void lock(void) {
	EnterCriticalSection(&cs);
    }
    void unlock(void) {
	LeaveCriticalSection(&cs);
    }
};

#else

class Mutex : public pthread_mutex_t {
public:
    Mutex(void) {
	pthread_mutex_init(this, NULL);
    }
    ~Mutex(void) {
	pthread_mutex_destroy(this);
    }
    void lock(void) {
	pthread_mutex_lock(this);
    }
    void unlock(void) {
	pthread_mutex_unlock(this);
    }
    bool trylock(void) {
	if (pthread_mutex_trylock(this) == EBUSY) 
	    return false;
	else
	    return true;
    }
};

#endif // !_WIN32

#endif // THREAD_H
