// $Id: training.cpp,v 1.11 2006/11/08 22:00:13 weesan Exp $

/*
 * training.cpp - Training dialog.
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	8/14/2005
 */

#ifdef HAVE_FLTK

#include <Fl/Fl.h>
#include <Fl/fl_draw.h>
#include <Fl/fl_ask.h>
#include "stdafx.h"
#include "graph_based_doc.h"
#include "graph_based_view.h"
#include "symbol.h"
#include "training.h"
#include "glue.h"
#include "directory.h"
#include "definition.h"
#include "scandir.h"
#include "debug.h"

#define TRAINING_WIDTH			300
#define TRAINING_HEIGHT			(300 + 30 + 30)
#define TRAINING_DIR			"training"
#define TRAINING_SYMBOL_DIR		DEF_DIR_PREFIX "\\symbols"
#define TRAINING_DEF_DIR		DEF_DIR_PREFIX "\\definitions"

// Used by training_next_sym() below
static Training *pcTraining = NULL;

static void cb_redraw(Fl_Widget *w, void *pvTraining) {
	Training *pcTraining = (Training *)pvTraining;
	pcTraining->view()->SendMessage(WM_KEYUP, VK_DELETE);
}

// Callback to process next button
static void cb_next_sym(Fl_Widget *w, void *pvTraining) {
	static bool bDone = false;
	Training *pcTraining = (Training *)pvTraining;

	// When done, terminate the app
	if (bDone) {
		exit(0);
	}
	
	// Move to next one
	pcTraining->next();

	// Check if it's done or not
	if (pcTraining->done()) {
		pcTraining->nextButton()->label("&Done");
		bDone = true;
	}

	// Redraw
	pcTraining->redraw();
}

// Callback to process escape button
static void cb_escape(Fl_Widget *w, void *) {
	if (fl_ask("Are you sure you want to quit training "
			   "before it's finished?")) {
		exit(0);
	}
}

//////////////////////////////////

void training_next_sym(void) {
	cb_next_sym(NULL, pcTraining);
}

//////////////////////////////////

Training::Training(CGraphBasedView *pcView, const char *pzcUser,
				   int iTestSamples, int iTrainingSamples, int iPause) :
	Fl_Window(0, 0, TRAINING_WIDTH, TRAINING_HEIGHT),
	pcView(pcView),
	iTotalSymbols(0),
	iTestSamples(iTestSamples),
	iTrainingSamples(iTrainingSamples),
	iTotalSamples(0),
	iPause(iPause),
	iPauseSample(0),
	iIndex(0),
	pcNextButton(NULL),
	pcProgress(NULL) {

	// Keep track of current instance of training for traning_next_sym()
	pcTraining = this;

	// Keep track of user
	snprintf(this->pzcUser, sizeof(this->pzcUser), "%s", pzcUser);
	// Create directories
	char pzcDir[_MAX_PATH];
	if (testSamples()) {
		snprintf(pzcDir, sizeof(pzcDir), "%s.%s", TRAINING_SYMBOL_DIR, user());
		Directory::create(pzcDir, Directory::cwd());
	}
	if (trainingSamples()) {
		snprintf(pzcDir, sizeof(pzcDir), "%s.%s", TRAINING_DEF_DIR, user());
		Directory::create(pzcDir, Directory::cwd());
	}

	// Read training symbols
	readTrainingSymbols();

	// Compute total symbols a user needs to draw
#ifdef TRAINING_RAW
	iTotalSymbols = RawSymbols::size();
#else
	iTotalSymbols = vector<Gif *>::size();
#endif

	// Compute total symbols a user needs to draw
	iTotalSamples = (testSamples() + trainingSamples()) * iTotalSymbols;

	// Compute the symbol where user can take a rest
	if (pause()) {
		iPauseSample = pause() * iTotalSymbols;
	}

	// Background
	box(FL_UP_BOX);
	color(FL_WHITE);
	// Add and setup a progress bar
	pcProgress = new Fl_Progress(5, h() - 60, w() - 10, 25, "Progress");
	pcProgress->maximum(iTotalSamples);
	// Add redraw button
	(new Fl_Button(5, h() - 30, 70, 25, "&Redraw"))->callback(cb_redraw, this);
	// Add next button
	pcNextButton = new Fl_Button(w() - 75, h() - 30, 70, 25, "&Next @>>");
	pcNextButton->callback(cb_next_sym, this);
	// Make it resizable
	//resizable(this);
	// End of widgets
	Fl_Window::end();
	// Show the window
	show();

	// Install a callback to handle ESC key
	callback(cb_escape);
}

void Training::readTrainingSymbols(void) {
#ifdef TRAINING_RAW
	Symbols cSymbols(TRAINING_DIR);
	for (bool bRet = cSymbols.firstFile(); bRet; bRet = cSymbols.nextFile()) {
		char pzcName[_MAX_PATH];
		snprintf(pzcName, sizeof(pzcName),
				 "%s\\%s", cSymbols.dir(), cSymbols.filename());
		RawSymbol *pcRawSymbol = glue_raw_symbol(pzcName);
		if (pcRawSymbol) {
			cSymbols.name(pzcName);
			pcRawSymbol->name(pzcName);
			push_back(pcRawSymbol);
		}
	}
#else
	ScanDir cDir(TRAINING_DIR, "*.gif");
	for (const char *c = cDir.first(); c; c = cDir.next()) {
		push_back(new Gif(cDir.path(), c));
	}
#endif
}

bool Training::instruction(void) {
	return (fl_ask("Instructions:\n"
				   "\n"
				   "\t1. Please draw naturally but with reasonable care.\n"
				   "\t2. Do not try to \"trick\" or \"break\" the system.\n"
				   "\t3. No overstroking, ie. do not go back and forth to fix things.  Use \"Redraw\" button if necessary.\n"
				   "\t4. To advance to next symbol, hit the SPACEBAR or click on \"Next\" button.\n"
				   "\t5. Take as many breaks as you like during your drawing.\n"
				   "\t6. Should you have any problems, let us know ASAP.\n"
				   "\n"
				   "If you are ready to proceed, please click 'Yes', otherwise, click 'No'.\n"
			));
}

void Training::proc(void) {
	if (fl_ask("Instructions:\n"
			   "\n"
			   "\t1. Please draw naturally but with reasonable care.\n"
			   "\t2. Do not try to \"trick\" or \"break\" the system.\n"
			   "\t3. No overstroking, ie. do not go back and forth to fix things.  Use \"Redraw\" button if necessary.\n"
			   "\t4. To advance to next symbol, hit the SPACEBAR or click on \"Next\" button.\n"
			   "\t5. Take as many breaks as you like during your drawing.\n"
			   "\t6. Should you have any problems, let us know ASAP.\n"
			   "\n"
			   "If you are ready to proceed, please click 'Yes', otherwise, click 'No'.\n"
				)) {
		Fl::run();
	} else {
		exit(1);
	}
}

void Training::next(void) {
	// Before going to next symbol, signal the view to save data and
	// clear the page
	char pzcFilename[_MAX_PATH];
	char *pzcDir = NULL;

	// Construct the fullpath of the file to be saved
	if (index() < testSamples() * totalSymbols()) {
		pzcDir = TRAINING_SYMBOL_DIR;
	} else {
		pzcDir = TRAINING_DEF_DIR;
	}
	snprintf(pzcFilename, sizeof(pzcFilename), "%s\\%s.%s\\%s.txt",
			 Directory::cwd(), pzcDir, user(),
			 (*this)[index() % totalSymbols()]->name());
	// Save current sketch
	if (view()->save(pzcFilename)) {
		// Delete current sketch
		view()->SendMessage(WM_KEYUP, VK_DELETE);

		// Increase the index
		if (iIndex < totalSamples()) {
			progress()->value(++iIndex);
			if (iIndex == pauseSample()) {
				fl_message("Please take a few minutes break before continue!");
			}
		}
	}
}

void Training::draw(void) {
	// Call parent's draw()
	Fl_Window::draw();

	if (done()) {
		label("Whew!");
		fl_color(FL_BLACK);
		fl_draw("Thank you :)", w() / 3, h() / 2);
	} else {
		char pzcName[128];

#ifdef TRAINING_RAW
		// Setup color and line width
		fl_color(FL_BLACK);
		fl_line_style(FL_SOLID, 2);
		// Get the current symbol
		RawSymbol *pcRawSymbol = (*this)[index() % RawSymbols::size()];

		// Get the name of the symbol
		snprintf(pzcName, sizeof(pzcName), "%s", pcRawSymbol->name());

		// Draw the raw symbol
		int iOffsetX = pcRawSymbol->centerX() - w() / 2;
		int iOffsetY = pcRawSymbol->centerY() - h() / 2;
		for (int j = 0; j < pcRawSymbol->size(); j++) {
			RawSegment &cRawSegment = (*pcRawSymbol)[j];
			fl_begin_line();
			for (int k = 0; k < cRawSegment.size(); k++) {
				RawPoint &cRawPoint = cRawSegment[k];
				fl_vertex(cRawPoint.x() - iOffsetX,
						  h() - (cRawPoint.y() - iOffsetY));
			}
			fl_end_line();
		}
#else
		Gif *pcGif = (*this)[index() % totalSymbols()];
		pcGif->draw(0, 0);
		snprintf(pzcName, sizeof(pzcName), "%s", pcGif->name());
#endif

		// Set the title
		char pzcTitle[128];
		snprintf(pzcTitle, sizeof(pzcTitle), "%s: %d - %s",
				 user(), index() + 1, pzcName);
//		snprintf(pzcTitle, sizeof(pzcTitle), "%s: %d/%d - %s",
//				 user(), index() + 1, totalSamples(), pzcName);
		label(pzcTitle);
	}
}

#endif // HAVE_FLTK