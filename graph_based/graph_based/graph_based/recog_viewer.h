// $Id: recog_viewer.h,v 1.2 2006/11/08 22:00:13 weesan Exp $

/*
 * recog_viewer.h - A recognizer viewer
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	12/18/2005
 */

#ifndef RECOG_VIEWER_H
#define RECOG_VIEWER_H

#ifdef HAVE_FLTK

#include "symbol.h"
#include "match.h"
#include <FL/Fl_Window.h>

class RecogViewer : public Fl_Window {
public:
	RecogViewer(const Symbol *pcSymbol, const Match *pcMatch);
	int run(void);
	void draw(void);
};

#endif // HAVE_FLTK

#endif // RECOG_VIEWER_H