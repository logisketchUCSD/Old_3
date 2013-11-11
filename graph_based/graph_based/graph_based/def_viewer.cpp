#ifdef HAVE_FLTK

#include <stdio.h>
#include <FL/Fl.h>
#include <FL/fl_draw.h>
#include <FL/Fl_button.h>
#include "def_viewer.h"
#include "debug.h"

#define DEF_VIEWER_WIDTH        500
#define DEF_VIEWER_HEIGHT		500
#define DEF_VIEWER_PEN_WIDTH	2

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

static void cb_prev_sym(Fl_Widget *w, void *pvDefViewer) {
	DefViewer *pcDefViewer = (DefViewer *)pvDefViewer;
	pcDefViewer->prevSymbol();
	pcDefViewer->redraw();
	if (pcDefViewer->parent()) {
		pcDefViewer->parent()->redraw();
	}
}

static void cb_next_sym(Fl_Widget *w, void *pvDefViewer) {
	DefViewer *pcDefViewer = (DefViewer *)pvDefViewer;
	pcDefViewer->nextSymbol();
	pcDefViewer->redraw();
	if (pcDefViewer->parent()) {
		pcDefViewer->parent()->redraw();
	}
}

static void cb_prev_def(Fl_Widget *w, void *pvDefsViewer) {
	DefsViewer *pcDefsViewer = (DefsViewer *)pvDefsViewer;
	pcDefsViewer->prevDef();
	pcDefsViewer->redraw();
}

static void cb_next_def(Fl_Widget *w, void *pvDefsViewer) {
	DefsViewer *pcDefsViewer = (DefsViewer *)pvDefsViewer;
	pcDefsViewer->nextDef();
	pcDefsViewer->redraw();
}

//////////////////////////////////

DefViewer::DefViewer(Fl_Window *pcParent, int x, int y, const Definition *pcDef) :
	Fl_Window(x, y, DEF_VIEWER_WIDTH, DEF_VIEWER_HEIGHT),
	pcParent(pcParent),
	pcDef(pcDef),
	iIndex(0) {
	// Background
	box(FL_UP_BOX);
	color(FL_WHITE);
	// Add some buttons
	(new Fl_Button(w() - 90 * 2, h() - 30,
				   85, 25, "&Prev @<"))->callback(cb_prev_sym, this);
	(new Fl_Button(w() - 90, h() - 30,
				   85, 25, "&Next @>"))->callback(cb_next_sym, this);
	// Make it resizable
	resizable(this);
	// End of widgets
	end();
	// Do not show here since this window is going to be a child window
	//show();
}

int DefViewer::run(void) {
	return (Fl::run());
}

void DefViewer::drawSymbol(Symbol *pcSymbol) {
	// Get the center of the symbol
	int x = 0, y = 0;
	pcSymbol->center(&x, &y);
	x = x - w() / 2;
	y = y - h() / 2;

	// Put a bounding box on the symbol
	int iMinX = pcSymbol->minX();
	int iMinY = pcSymbol->minY();
	int iMaxX = pcSymbol->maxX();
	int iMaxY = pcSymbol->maxY();
	int xSteps = (iMaxX - iMinX + SYMBOL_CHUCK_SIZE - 1) / SYMBOL_CHUCK_SIZE;
	int ySteps = (iMaxY - iMinY + SYMBOL_CHUCK_SIZE - 1) / SYMBOL_CHUCK_SIZE;

	fl_color(FL_GRAY);
	fl_line_style(FL_DOT);
	fl_rect(iMinX - x, iMinY - y, iMaxX - iMinX, iMaxY - iMinY);
	for (int i = iMinX; i < iMaxX; i += xSteps) {
		fl_line(i - x, iMinY - y, i - x, iMaxY - y);
	}
	for (int i = iMinY; i < iMaxY; i += ySteps) {
		fl_line(iMinX - x, i - y, iMaxX - x, i - y);
	}

	// Start drawing the symbol
	for (int k = 0; k < pcSymbol->size(); k++) {
		char pzcSegNo[32];
		snprintf(pzcSegNo, sizeof(pzcSegNo), "%d", k);
		// Get the segment
		MySegment *pcSegment = pcSymbol->getSegment(k);
		if (pcSegment == NULL) {
			continue;
		}
		// Set the color for each segment
		fl_color(piColors[k % iTotalColors]);
		fl_line_style(FL_SOLID, DEF_VIEWER_PEN_WIDTH);

		switch (pcSegment->Type()) {
		case LINE: {
			int x1 = pcSegment->StartPoint().x - x; 
			int y1 = h() - (pcSegment->StartPoint().y - y);
			int x2 = pcSegment->EndPoint().x - x;
			int y2 = h() - (pcSegment->EndPoint().y - y);
			fl_line(x1, y1, x2, y2);
			fl_draw(pzcSegNo, x1 + 5, y1 - 5);
			// The following works just fine
			/*
			fl_begin_line();
			fl_vertex(pcSegment->StartPoint().x,
					  h() - pcSegment->StartPoint().y);
			fl_vertex(pcSegment->EndPoint().x,
					  h() - pcSegment->EndPoint().y);
			fl_end_line();
			*/
			break;
		}
		case ARC:
			fl_begin_line();
			fl_arc(pcSegment->Center().x - x,
				   h() - (pcSegment->Center().y - y),
				   pcSegment->Radius(),
				   Rad2Deg(pcSegment->SmallAngle()),
				   Rad2Deg(pcSegment->BigAngle()));
			fl_end_line();
			fl_draw(pzcSegNo,
					pcSegment->StartPoint().x - x + 5,
					h() - (pcSegment->StartPoint().y - y) - 5);
			break;
		default:
			break;
		}
	}
}

void DefViewer::draw(void) {
	Fl_Window::draw();

	char pzcTitle[128];
	snprintf(pzcTitle, sizeof(pzcTitle), "%s (%d/%d)",
			 def()->name(), index() + 1, def()->size());
	fl_draw(pzcTitle, 5, 20);
	drawSymbol((*def())[index()]);
}

//////////////////////////////////

DefsViewer::DefsViewer(void) :
	Fl_Window(DEF_VIEWER_WIDTH * 2 + 2, DEF_VIEWER_HEIGHT + 50),
	iIndex(0),
	pcDefViewer1(NULL),
	pcDefViewer2(NULL) {

	if (cDefs.size()) {
		// Allocate 2 definition viewers
		pcDefViewer1 = new DefViewer(this, 0, 0, cDefs[index()]);
		pcDefViewer2 = new DefViewer(this, DEF_VIEWER_WIDTH + 2, 0, cDefs[index()]);
		assert(pcDefViewer1 != NULL && pcDefViewer2 != NULL);
		// Background
		box(FL_FLAT_BOX);
		color(FL_WHITE);
		// Add some buttons
		(new Fl_Button(w() - 90 * 2, h() - 30,
					   85, 25, "&Prev @<<"))->callback(cb_prev_def, this);
		(new Fl_Button(w() - 90, h() - 30,
					   85, 25, "&Next @>>"))->callback(cb_next_def, this);
	}

	// Make it resizable
	resizable(this);
	// End of widgets
	end();
	// Display the window
	show();
}

int DefsViewer::run(void) {
	return (Fl::run());
}

void DefsViewer::draw(void) {
	Fl_Window::draw();

	if (cDefs.size()) {
		int iIndex1 = defViewer1()->index();
		int iIndex2 = defViewer2()->index();
		const char *pzcDrawOrder1=(*defViewer1()->def())[iIndex1]->drawOrder();
		const char *pzcDrawOrder2=(*defViewer2()->def())[iIndex2]->drawOrder();
		char pzcTitle[256];
		snprintf(pzcTitle, sizeof(pzcTitle),
				 "%s | %s", pzcDrawOrder1, pzcDrawOrder2);
		label(pzcTitle);
	} else {
		label("No definitions!");
	}
}

int DefsViewer::handle(int iEvent) {
	switch (iEvent) {
	case FL_KEYBOARD:
		if (Fl::event_state() == FL_CTRL) {
			switch (Fl::event_key()) {
			case 'n':
				nextDef();
				redraw();
				break;
			case 'p':
				prevDef();
				redraw();
				break;
			}
		}
		break;
	default:
		break;
	}

	return (Fl_Window::handle(iEvent));
}

#endif // HAVE_FLTK