// $Id: training.h,v 1.6 2006/11/08 22:00:13 weesan Exp $

/*
 * training.h - Training dialog.
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	8/14/2005
 */

#ifndef TRAINING_H
#define TRAINING_H

#ifdef HAVE_FLTK

#include <Fl/Fl_Window.h>
#include <Fl/Fl_Button.h>
#include <Fl/Fl_GIF_Image.h>
#include <Fl/Fl_Progress.h>
#include <vector>
#include "thread.h"
#include "debug.h"

using namespace std;

// Forward declaration
class CGraphBasedView;

// A function will be called in CGraphBasedView::OnKeyUp()
void training_next_sym(void);

#ifdef TRAINING_RAW

class RawPoint {
private:
	int iX, iY;
public:
	RawPoint(int iX, int iY) : iX(iX), iY(iY) {
	}
	int x(void) const {
		return (iX);
	}
	int y(void) const {
		return (iY);
	}
};

class RawSegment : public vector<RawPoint> {
};

class RawSymbol : public vector<RawSegment> {
private:
	int iMinX, iMinY;
	int iMaxX, iMaxY;
	char pzcName[64];

public:
	RawSymbol(void) :
		iMinX(10000),
		iMinY(10000),
		iMaxX(0),
		iMaxY(0) {
		pzcName[0] = 0;
	}
	int centerX(void) const {
		return ((iMaxX - iMinX) / 2 + iMinX);
	}
	int centerY(void) const {
		return ((iMaxY - iMinY) / 2 + iMinY);
	}
	const char *name(void) const {
		return (pzcName);
	}
	void name(const char *_pzcName) {
		snprintf(pzcName, sizeof(pzcName), "%s", _pzcName);
	}
	void push_back(RawSegment &cRawSegment) {
		for (int i = 0; i < cRawSegment.size(); i++) {
			RawPoint &cRawPoint = cRawSegment[i];
			iMinX = min(iMinX, cRawPoint.x());
			iMinY = min(iMinY, cRawPoint.y());
			iMaxX = max(iMaxX, cRawPoint.x());
			iMaxY = max(iMaxY, cRawPoint.y());
		}
		vector<RawSegment>::push_back(cRawSegment);
	}
};

class RawSymbols : public vector<RawSymbol *> {
public:
	~RawSymbols(void) {
		for (int i = 0; i < size(); i++) {
			delete (*this)[i];
		}
	}
};

#else

class Gif : public Fl_GIF_Image {
private:
	char pzcPath[_MAX_PATH];
	char pzcFile[64];
	char pzcName[64];

public:
	Gif(const char *_pzcPath, const char *_pzcFile) :
		Fl_GIF_Image(_pzcPath) {
		snprintf(pzcPath, sizeof(pzcPath), _pzcPath);
		snprintf(pzcFile, sizeof(pzcFile), _pzcFile);

		// Use pzcFile rather than _pzcFile below
		sscanf(pzcFile, "%64[^.]", pzcName);
	}
	const char *path(void) const {
		return (pzcPath);
	}
	const char *file(void) const {
		return (pzcFile);
	}
	const char *name(void) const {
		return (pzcName);
	}
};

#endif

class Training : public Thread, public Fl_Window,
#ifdef TRAINING_RAW
	public RawSymbols
#else
	public vector<Gif *>
#endif
{
private:
	CGraphBasedView *pcView;
	char pzcUser[64];
	int iTotalSymbols;
	int iTestSamples;
	int iTrainingSamples;
	int iTotalSamples;
	int iPause;
	int iPauseSample;
	int iIndex;
	Fl_Button *pcNextButton;
	Fl_Progress *pcProgress;

private:
	void readTrainingSymbols(void);
	bool instruction(void);

protected:
	void proc(void);
	void cleanUp(void) {
	}

public:
	Training(CGraphBasedView *pcView, const char *pzcUser,
			 int iTestSamples, int iTrainingSamples, int iPause);
	CGraphBasedView *view(void) const {
		return (pcView);
	}
	const char *user(void) const {
		return (pzcUser);
	}
	int totalSymbols(void) const {
		return (iTotalSymbols);
	}
	int testSamples(void) const {
		return (iTestSamples);
	}
	int trainingSamples(void) const {
		return (iTrainingSamples);
	}
	int totalSamples(void) const {
		return (iTotalSamples);
	}
	int pause(void) const {
		return (iPause);
	}
	int pauseSample(void) const {
		return (iPauseSample);
	}
	int index(void) const {
		return (iIndex);
	}
	Fl_Button *nextButton(void) const {
		return (pcNextButton);
	}
	Fl_Progress *progress(void) const {
		return (pcProgress);
	}
	void next(void);
	bool done(void) {
		return (index() == totalSamples());
	}
	bool ok(void) const {
		return (true);
	}
	void draw(void);
};

#endif // HAVE_FLTK

#endif // TRAINING_H