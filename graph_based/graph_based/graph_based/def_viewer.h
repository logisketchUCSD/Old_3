#ifndef DEF_VIEWER_H
#define DEF_VIEWER_H

#ifdef HAVE_FLTK

#include <FL/Fl_Window.h>
#include "definition.h"

class DefViewer : public Fl_Window {
private:
	Fl_Window *pcParent;
	const Definition *pcDef;
	int iIndex;

private:
	void drawSymbol(Symbol *pcSymbol);

public:
	DefViewer(Fl_Window *pcParent, int x, int y, const Definition *pcDef);
	Fl_Window *parent(void) const {
		return (pcParent);
	}
	const Definition *def(void) const {
		return (pcDef);
	}
	int index(void) const {
		return (iIndex);
	}
	int run(void);
	void draw(void);
	void prevSymbol(void) {
		if (def()->size() == 0) {
			iIndex = 0;
		} else {
			if (--iIndex < 0) {
				iIndex = def()->size() - 1;
			}
		}
	}
	void nextSymbol(void) {
		iIndex = (iIndex + 1) % def()->size();
	}
	void assign(Definition *_pcDef) {
		pcDef = _pcDef;
		iIndex = 0;
		redraw();
	}
};

class DefsViewer : public Fl_Window {
private:
	int iIndex;
	DefViewer *pcDefViewer1, *pcDefViewer2;

public:
	DefsViewer(void);
	~DefsViewer(void) {
		if (pcDefViewer1) {
			delete pcDefViewer1;
		}
		if (pcDefViewer2) {
			delete pcDefViewer2;	
		}
	}
	int index(void) const {
		return (iIndex);
	}
	DefViewer *defViewer1(void) const {
		return (pcDefViewer1);
	}
	DefViewer *defViewer2(void) const {
		return (pcDefViewer2);
	}
	bool ok(void) const {
		return (defViewer1() != NULL && defViewer2() != NULL);
	}
	int run(void);
	void draw(void);
	int handle(int iEvent);
	void prevDef(void) {
		if (cDefs.size() == 0) {
			iIndex = 0;
		} else {
			if (--iIndex < 0) {
				iIndex = cDefs.size() - 1;
			}
		}
		defViewer1()->assign(cDefs[iIndex]);
		defViewer2()->assign(cDefs[iIndex]);
	}
	void nextDef(void) {
		iIndex = (iIndex + 1) % cDefs.size();
		defViewer1()->assign(cDefs[iIndex]);
		defViewer2()->assign(cDefs[iIndex]);
	}
};

#endif // HAVE_FLTK

#endif // DEF_VIEWER_H