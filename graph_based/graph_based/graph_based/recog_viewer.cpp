// $Id

/*
 * recog_viewer.cpp - A recognizer viewer
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	12/18/2005
 */

#ifdef HAVE_FLTK

#include <FL/Fl.h>
#include <FL/fl_draw.h>
#include <FL/Fl_button.h>
#include "recog_viewer.h"
#include "symbol_viewer.h"
#include "def_viewer.h"

RecogViewer::RecogViewer(const Symbol *pcSymbol,
						 const Match *pcMatch) :
	Fl_Window(0, 0) {
	// Background
	box(FL_FLAT_BOX);
	color(FL_WHITE);

	assert(pcSymbol != NULL && pcMatch != NULL);

	SymbolViewer *pcSymbolViewer = new SymbolViewer(0, 0, pcSymbol);
	DefViewer *pcDefViewer = new DefViewer(NULL, pcSymbolViewer->w(), 0,
										   cDefs[pcMatch->top().def()]);
	assert(pcDefViewer != NULL && pcSymbolViewer != NULL);

	//Resize the window
	size(pcDefViewer->w() + pcSymbolViewer->w(),
		 max(pcDefViewer->h(), pcSymbolViewer->h()));

	// Make it resizable
	resizable(this);
	// End of widgets
	end();
	// Display the window
	show();
}

int RecogViewer::run(void) {
	return (Fl::run());
}

void RecogViewer::draw(void) {
	Fl_Window::draw();
}

#endif // HAVE_FLTK