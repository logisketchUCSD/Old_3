// $Id: symbol_viewer.cpp,v 1.3 2006/11/08 22:00:13 weesan Exp $

/*
 * symbol_viewer.cpp - A symbol viewer
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/28/2005
 */

#ifdef HAVE_FLTK

#include <FL/Fl.h>
#include <FL/fl_draw.h>
#include <FL/Fl_button.h>
#include "symbol_viewer.h"
#include "matrix.h"
#include "debug.h"

#define SYMBOL_VIEWER_WIDTH			400
#define SYMBOL_VIEWER_HEIGHT		400
#define SYMBOL_VIEWER_PEN_WIDTH		2
#define SYMBOL_VIEWER_SCALE_FACTOR	0.9

static int piColors[] = {
//	FL_GRAY,
	FL_BLACK,
	FL_RED,
	FL_GREEN,
//	FL_YELLOW,
	FL_BLUE,
	FL_MAGENTA,
	FL_CYAN,
	FL_DARK_RED,
	FL_DARK_GREEN,
	FL_DARK_YELLOW,
	FL_DARK_BLUE,
	FL_DARK_MAGENTA,
	FL_DARK_CYAN
};
static int iTotalColors = sizeof(piColors) / sizeof(piColors[0]);

static void cb_restore(Fl_Widget *w, void *pvSymbolsViewer) {
	SymbolsViewer *pcSymbolsViewer = (SymbolsViewer *)pvSymbolsViewer;
	pcSymbolsViewer->defaultSize();
	pcSymbolsViewer->redraw();
}

static void drawSymbol(const Symbol *pcSymbol, int iWidth, int iHeight) {
	int x1 = pcSymbol->minX();
	int y1 = pcSymbol->minY();
	int x2 = pcSymbol->maxX();
	int y2 = pcSymbol->maxY();

	double xScale = (float)iWidth  / (x2 - x1) * SYMBOL_VIEWER_SCALE_FACTOR;
	double yScale = (float)iHeight / (y2 - y1) * SYMBOL_VIEWER_SCALE_FACTOR;

	// Translate the symbol back to origin and scale it
	Matrix m = MatrixScaling(xScale, yScale) *
			   MatrixTranslation(-x1 + (x2 - x1) * (1 - SYMBOL_VIEWER_SCALE_FACTOR) / 2,
								 -y1 + (y2 - y1) * (1 - SYMBOL_VIEWER_SCALE_FACTOR) / 2);

	Matrix p1(3, 1), p2(3, 1), pp1(3, 1), pp2(3, 1);
	double r = 0;

	for (int i = 0; i < pcSymbol->size(); i++) {
		MySegment *pcSegment = (*pcSymbol)[i];
		char pzcSegNo[32];
		snprintf(pzcSegNo, sizeof(pzcSegNo), "%d", i);

		switch (pcSegment->Type()) {
		case LINE:
			// Store original end points of the line into p1 and p2
			p1.data(3,
					pcSegment->StartPoint().x, pcSegment->StartPoint().y, 1.0);
			p2.data(3,
					pcSegment->EndPoint().x, pcSegment->EndPoint().y, 1.0);
			// pp1 & pp2 are the new end points after transformation
			pp1 = m * p1;
			pp2 = m * p2;
			// Draw the line
			fl_line(pp1.x(), iHeight - pp1.y(), pp2.x(), iHeight - pp2.y());
			fl_draw(pzcSegNo, pp1.x() + 5, iHeight - pp1.y() + 5);
			break;
		case ARC:
			// Store center of the arc into p1
			p1.data(3, pcSegment->Center().x, pcSegment->Center().y, 1.0);
			// Store end points of the arc into p2
			p2.data(3,
					pcSegment->StartPoint().x, pcSegment->StartPoint().y, 1.0);
			// pp1 is the new center after transformation
			pp1 = m * p1;
			// pp2 is the new segment end points after transformation
			pp2 = m * p2;
			// r is the new radius after transformation
			r = pcSegment->Radius() * min(xScale, yScale);
			// Draw the arc
			fl_begin_line();
			fl_arc(pp1.x(),
				   iHeight - pp1.y(),
				   r,
				   Rad2Deg(pcSegment->SmallAngle()),
				   Rad2Deg(pcSegment->BigAngle()));
			fl_end_line();
			fl_draw(pzcSegNo, pp2.x() + 5, iHeight - pp2.y() + 5);
			break;
		default:
			break;
		}
	}
}

//////////////////////////////////

SymbolViewer::SymbolViewer(int x, int y, const Symbol *pcSymbol) :
	pcSymbol(pcSymbol),
	Fl_Window(x, y, SYMBOL_VIEWER_WIDTH , SYMBOL_VIEWER_HEIGHT) {
	assert(pcSymbol != NULL);

	// Background
	box(FL_UP_BOX);
	color(FL_WHITE);
	// Make it the same size of the symbol
	size((pcSymbol->maxX() - pcSymbol->minX()) * (2.0 - SYMBOL_VIEWER_SCALE_FACTOR),
		 (pcSymbol->maxY() - pcSymbol->minY()) * (2.0 - SYMBOL_VIEWER_SCALE_FACTOR));
	// Make it resizable
	resizable(this);
	// End of widgets
	end();
	// Display the window
	//show();
}

int SymbolViewer::run(void) {
	return (Fl::run());
}

void SymbolViewer::draw(void) {
	Fl_Window::draw();
	fl_color(FL_BLACK);
	fl_line_style(FL_SOLID, SYMBOL_VIEWER_PEN_WIDTH);
	// XXX: for some reasons, the font size has to be set, otherwise,
	// crash on call fl_draw() in drawSymbol() above
	fl_font(FL_HELVETICA, 11);
	drawSymbol(symbol(), w(), h());
}

//////////////////////////////////

SymbolsViewer::SymbolsViewer(const Symbols *pcSymbols) :
	Fl_Window(SYMBOL_VIEWER_WIDTH , SYMBOL_VIEWER_HEIGHT),
	pcSymbols(pcSymbols) {
	assert(pcSymbols != NULL);

	// Background
	//box(FL_UP_BOX);
	color(FL_WHITE);

	// Set the size based on the size of first symbol
	defaultSize();

	(new Fl_Button(w() - 80, h() - 30, 75, 25,
				   "&Restore"))->callback(cb_restore, this);

	// Make it resizable
	resizable(this);
	// End of widgets
	end();
	// Display the window
	show();
}

int SymbolsViewer::run(void) {
	return (Fl::run());
}

void SymbolsViewer::draw(void) {
	Fl_Window::draw();

	for (int i = 0; i < symbols()->size(); i++) {
		fl_color(piColors[i % iTotalColors]);
		fl_line_style(FL_SOLID, SYMBOL_VIEWER_PEN_WIDTH);
		Symbol *pcSymbol = (*symbols())[i];
		drawSymbol(pcSymbol, w(), h());
	}
}

void SymbolsViewer::defaultSize(void) {
	Symbol *pcSymbol = (*pcSymbols)[0];
	size(pcSymbol->maxX() - pcSymbol->minX(),
		 pcSymbol->maxY() - pcSymbol->minY());
}

#endif // HAVE_FLTK