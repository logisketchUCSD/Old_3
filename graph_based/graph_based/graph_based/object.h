// $Id: object.h,v 1.1 2005/06/22 23:05:42 weesan Exp $

/*
 * object.h - Base object for all class
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	6/9/2005
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include "stdafx.h"

using namespace std;

class Object {
	friend std::ostream &operator<<(std::ostream &os, const Object &o);
	friend std::istream &operator>>(std::istream &is, Object &o);

private:
	const char *pzcName;

public:
	Object(const char *pzcName) : pzcName(pzcName) {
	}
	const char *name(void) const {
		return (pzcName);
	}
	// XXX: VC++ 6.0 doesn't like write() and read() being protected :(
	virtual void write(std::ostream &os) const = 0;
	virtual void read(std::istream &is) = 0;
};

inline std::ostream &operator<<(std::ostream &os, const Object &o) {
	o.write(os);
	return (os);
}

inline std::istream &operator>>(std::istream &is, Object &o) {
	o.read(is);
	return (is);
}

#endif // OBJECT_H