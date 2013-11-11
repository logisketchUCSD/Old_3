// $Id: symbol_viewer.h,v 1.3 2006/11/08 22:00:13 weesan Exp $

/*
 * symbol_viewer.h - A symbol viewer
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/28/2005
 */

#ifndef SYMBOL_VIEWER_H
#define SYMBOL_VIEWER_H

#ifdef HAVE_FLTK

#include <FL/Fl_Window.h>
#include "symbol.h"

class SymbolViewer : public Fl_Window {
private:
	const Symbol *pcSymbol;

public:
	SymbolViewer(int x, int y, const Symbol *pcSymbol);
	const Symbol *symbol(void) const {
		return (pcSymbol);
	}
	int run(void);
	void draw(void);
};

class SymbolsViewer : public Fl_Window {
private:
	const Symbols *pcSymbols;

public:
	SymbolsViewer(const Symbols *pcSymbols);
	const Symbols *symbols(void) const {
		return (pcSymbols);
	}
	int run(void);
	void draw(void);
	void defaultSize(void);
};

#endif // HAVE_FLTK

#endif // SYMBOL_VIEWER_H