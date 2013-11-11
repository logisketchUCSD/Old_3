// $Id: glue.cpp,v 1.28 2007/04/17 23:45:20 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <gl\gl.h>
#include "stdafx.h"
#include "glue.h"
#include "definition.h"
#include "utils.h"
#include "dist_prob.h"
#include "extern.h"
#include "bgi_font.h"
#include "segment.h"
#include "recognizer.h"
#include "sound.h"
#include "infix2postfix.h"
#include "debug.h"

#define GLUE_N_BEST				10

/*
#define RANDOM_MATCH
#define PROBABILISTIC_MATCH
#define SYSTEMATIC_MATCH
#define SORT_MATCH
*/

#define PROB_MATCH_NO_BETTER	20
#define PROB_MATCH_FAILSAFE		100

using namespace std;

static fstream *glue_ifs = NULL;
static BgiFont cBgiFont("fonts\\litt.chr");

//////////////////////////////////

int glue_train(CView *pcView, SketchList *pcSketches) 
{
	int i;
	int success = -1;
	Definition *Defn=new Definition();

	for(i = 0; i < pcSketches->GetSize(); i++) 
	{
		Symbol *symbol = glue_new_symbol((*pcSketches)[i]);
		if (!symbol) {
			success = -1;
			break;
		}
		if ((success = Defn->AddTrainSymbol(symbol)) != 0) {
			delete symbol;
			break;
		};
	}

/*
	InputLine cInputLine("Please input symbol name: ");	
	cInputLine.DoModal();
*/

	do {
		if (success != 0) {
			// Switch to problematic sketch
			CurrentSketch = Sketches[i];
			// Cause it to redraw
			pcView->RedrawWindow();
		}

		// Then display possible helpful message to fix the problem
		switch (success) {
		case 0:
			// Do nothing here and proceed
			break;
		case 1:
			DEBUG("Training failed, primitive count check failure "
				  "at sketch %d.  Please try again!  Thank you.", i + 1);
			break;
		case 2:
			DEBUG("Training failed, primitive type check failure "
				  "at sketch %d.  Please try again!  Thank you.", i + 1);
			break;
		default:
			if (glue_sketches_empty(pcSketches)) {
				DEBUG("Nothing to be trained!");
			} else if (glue_sketch_empty(CurrentSketch)) {
				DEBUG("Please delete current blank sketch and train again!");
			} else {
				DEBUG("Training failed.  Please delete all tranining samples and "
					  "try again!  Thank you.");
			}
			break;
		}

		if (success != 0) {
			delete Defn;
			break;
		}

		// Prompt for a name
		CFileDialog d(TRUE);
		if (d.DoModal() == IDOK) {
			char name[_MAX_PATH];
			// Got it
			snprintf(name, sizeof(name), "%s", (LPCTSTR)d.GetFileTitle());
			// Give a name for the definition symbol
			Defn->name(name);
			Defn->process();
			// Write the raw data to a file
			if (!glue_write_raw_data(name)) {
				DEBUG("Failed to write to %s", name);
			}
			cDefs.push_back(Defn);
		} else {
			// Didn't get it, return error
			success = -1;
		}
	} while (false);

	return (success);
}

int glue_recognize(RawSketch *pcSketch) 
{
	// Parse the sketch and create a new symbol
	Symbol *symbol = glue_new_symbol(pcSketch);
	if (symbol == NULL) {
		return (0);
	}

	// Try to match the symbol against those in definition
	printf("----------\n");
	Recognizer cRecognizer;
	cRecognizer.recognize(symbol, "Test symbol", Match::STOCHASTIC, GLUE_N_BEST);

	// XXX: Comment out sort matching for now since it would actually
	// re-order the segments in both test symbol and symbols in definitions,
	// which would screw up other matching algorithm
	//cRecognizer.recognize(symbol, "Test symbol", Match::SORT, GLUE_N_BEST);

	cRecognizer.recognize(symbol, "Test symbol", Match::ERROR_DRIVEN, GLUE_N_BEST);
	cRecognizer.recognize(symbol, "Test symbol", Match::GREEDY, GLUE_N_BEST);
	cRecognizer.recognize(symbol, "Test symbol", Match::DISTANCE, GLUE_N_BEST);

	// One with GUI
	//cRecognizer.recognize(symbol, "Test symbol", Match::STOCHASTIC);

	// Done with the symbol
	delete symbol;

	return(1);
}

static RawSketch *glue_read_first(const char *pzcName) {
	assert(pzcName != NULL);

	// If some application does not call glue_read_next() till the end,
	// in this case, glue_ifs is non-NULL, so, free it here
	if (glue_ifs != NULL) {
		delete glue_ifs;
		glue_ifs = NULL;
	}

	RawSketch *pcRawSketch = NULL;
	// Open the file
	glue_ifs = new fstream(pzcName, ios::in);
	// Read from the file
	if (glue_ifs && glue_ifs->is_open() && (pcRawSketch = new RawSketch)) {
		pcRawSketch->Read((ifstream &)*glue_ifs);
		return (pcRawSketch);
	} else {
		return (NULL);
	}
}

static RawSketch *glue_read_next(void) {
	RawSketch *pcRawSketch = NULL;

	if (glue_ifs && !glue_ifs->eof() && (pcRawSketch = new RawSketch)) {
		pcRawSketch->Read((ifstream &)*glue_ifs);
		// Last one might be relundant due to \n at the end
		if (pcRawSketch->Strokes.GetSize() == 0) {
			delete pcRawSketch;
			pcRawSketch = NULL;
		}
	}

	// No more raw data, free the fstream
	if (!pcRawSketch) {
		delete glue_ifs;
		glue_ifs = NULL;
	}

	return (pcRawSketch);
}

bool glue_write_raw_data(const char *pzcName) {
	// Append the name with .txt and become the filename
	char pzcFilename[_MAX_PATH];
	snprintf(pzcFilename, sizeof(pzcFilename), "%s.txt", pzcName);
	// Always append the file instead of overwriting it
	fstream ofs(pzcFilename, ios::out | ios::app);
	if (!ofs) {
		return (false);
	}

	// Loop through all raw sketches and save them
	for (int i = 0; i < Sketches.GetSize(); i++) {
		RawSketch *pcRawSketch = Sketches[i];
		pcRawSketch->Write((ofstream &)ofs);
	}

	return (true);
}

bool glue_read_raw_data(const char *pzcName) {
	// Remove all current sketches
	Sketches.Clear();
	stroke_data = NULL;

	RawSketch *pcRawSketch = NULL;
	for (pcRawSketch = glue_read_first(pzcName);
		 pcRawSketch;
		 pcRawSketch = glue_read_next()) {
		// Append the raw sketch into the list
		Sketches.InsertEnd(pcRawSketch);
	}

	if (Sketches.GetSize() &&
		(CurrentSketch = Sketches[Sketches.GetSize() - 1]) &&
		CurrentSketch->Strokes.GetSize()) {
		stroke_data = CurrentSketch->Strokes[CurrentSketch->Strokes.GetSize()-1];
	} else {
		CurrentSketch = NULL;
		stroke_data = NULL;
	}

	// After reading raw sketch, it always starts as a new stroke
	StartNewStroke = 1;

	return (true);
}

Symbol *glue_first_symbol(const char *pzcName) {
	RawSketch *pcRawSketch = glue_read_first(pzcName);
	if (pcRawSketch) {
		Symbol *pcSymbol = glue_new_symbol(pcRawSketch);
		delete pcRawSketch;
		return (pcSymbol);
	} else {
		return (NULL);
	}
}

Symbol *glue_next_symbol(void) {
	RawSketch *pcRawSketch = glue_read_next();
	if (pcRawSketch) {
		Symbol *pcSymbol = glue_new_symbol(pcRawSketch);
		delete pcRawSketch;
		return (pcSymbol);
	} else {
		return (NULL);
	}
}

Symbol *glue_new_symbol(RawSketch *pcSketch) {
	int j, k;
	Segment *seg;
	Circle_Dir arcdir;
	double arcSang,arcEang;
	MySegment* segment;

	Symbol* symbol= new Symbol;
	if (!symbol) {
		return (NULL);
	}

	for(j=0; j<pcSketch->Strokes.GetSize(); j++)
	{
		
		for(k=0; k<pcSketch->Strokes[j]->seglist.GetSize(); k++)
		{
			seg = pcSketch->Strokes[j]->seglist[k];

			if(seg->arc.dir==1) arcdir=CCW;
			else                arcdir=CW; 

			arcSang=seg->arc.real_sang;
			arcEang=seg->arc.real_eang;
			assert(arcSang>=0.0 && arcEang>=0.0 && arcSang<=2*PI && arcEang<=2*PI);

			segment = new MySegment(seg->line.sx, seg->line.sy,
								    seg->line.ex, seg->line.ey,
								    seg->arc.cx, seg->arc.cy,
									seg->arc.rad, arcSang, arcEang, arcdir,
									seg->type == LINE_SEG ? LINE : ARC);
			assert(segment != NULL);
			symbol->Add_Segment( segment);
			/*
			printf("%d: %d - %.02f %.02f %.02f %.02f, %.02f %.02f %.02f\n",
				k,
				seg->type,
				seg->line.sx, seg->line.sy,
				seg->line.ex, seg->line.ey,
				seg->arc.cx, seg->arc.cy, seg->arc.rad);
			*/
		}
	}

	symbol->Process();

	if (symbol->size() == 0) {
		delete symbol;
		return (NULL);
	} else {
		return (symbol);
	}
}

#ifdef TRAINING_RAW
RawSymbol *glue_raw_symbol(const char *pzcName) {
	RawSymbol *pcRawSymbol = NULL;
	RawSketch *pcSketch = NULL;

	do {
		if ((pcSketch = glue_read_first(pzcName)) == NULL) {
			break;
		}

		if ((pcRawSymbol = new RawSymbol) == NULL) {
			delete pcSketch;
			break;
		}

		for (int i = 0; i < pcSketch->Strokes.GetSize(); i++) {
			RawSegment cRawSegment;
			for (int j = 0; j < pcSketch->Strokes[i]->EndData; j++) {
				InkData &cInkData = pcSketch->Strokes[i]->Ink[j];
				cRawSegment.push_back(RawPoint(cInkData.P.x,
											   cInkData.P.y));
			}
			pcRawSymbol->push_back(cRawSegment);
		}

	} while (false);

	return (pcRawSymbol);
}
#endif

static int draw_char(int x, int y, char zcChar, float fScale = 1.0) {
	const BgiFontChar cChar = cBgiFont[zcChar];

	// Black color
	glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < cChar.size(); i++) {
		switch (cChar[i].opCode()) {
		case BgiFontStroke::MOVE:
			glEnd();
			glBegin(GL_LINE_STRIP);
			// Deliberate fall thru
		case BgiFontStroke::DRAW:
			glVertex2f(x + cChar[i].x() * fScale, 
					   y + cChar[i].y() * fScale);
			break;
		case BgiFontStroke::DONE:
		    break;
		default:
		    break;
		}
    }
    glEnd();

    return ((int)(cChar.width() * fScale));
}

static void draw_string(int x, int y, const char *pzcString,
						float fScale = 1.0) {
    int iOrigX = x;
    for (const char *p = pzcString; p && *p; p++) {
		x += draw_char(x, y, *p, fScale);
    }
}

void glue_display_segment_no(void) {
	int iCount = 0;

	for (int i = 0; i < CurrentSketch->Strokes.GetSize(); i++) {
		for (int j = 0; j < CurrentSketch->Strokes[i]->seglist.GetSize(); j++) {
			Segment *pcSegment = CurrentSketch->Strokes[i]->seglist[j];
			int x = 0, y = 0;

			switch (pcSegment->type) {
			case LINE_SEG:
				x = (int)pcSegment->line.sx;
				y = (int)pcSegment->line.sy;
				x += 10;
				break;
			case ARC_SEG:
				x = (int)(pcSegment->arc.cx + 
						  pcSegment->arc.rad * cos(pcSegment->arc.real_sang));
				y = (int)(pcSegment->arc.cy +
						  pcSegment->arc.rad * sin(pcSegment->arc.real_sang));
				x -= 10;
				break;
			default:
				break;
			}

			char pzcBuffer[128];
			snprintf(pzcBuffer, sizeof(pzcBuffer), "%d", iCount);
			draw_string(x, y + 5, pzcBuffer, 1.5);
			iCount++;
		}
	}
}

/*
#include "pc8x16.c"

void glue_display_segment_no(void) {
	const char str[] = "1234567890";

	static GLuint base = 0;
	if (!base) {
		base = glGenLists(256);
		for (int i = 0; i < 256; i++) {
			glNewList(base + i, GL_COMPILE);
			glBitmap(8, 16, 0, 2, 10, 0, puc8x16 + i * 16);
			glEndList();
		}
	}

	glListBase(base);
	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2f(600, 500);
	glRotatef(90, 0, 0, 1);
	glCallLists(strlen(str), GL_BYTE, str);
}
*/

void glue_display_segment_coord(int x, int y) {
	int iCount = 0;
	double dTotalLength = 0;
	int i;

	// Compute total length
	// XXX: Only good for LINE not for ARC yet
	for (i = 0; i < CurrentSketch->Strokes.GetSize(); i++) {
		for (int j = 0; j < CurrentSketch->Strokes[i]->seglist.GetSize(); j++) {
			Segment *pcSegment = CurrentSketch->Strokes[i]->seglist[j];
			dTotalLength += distance(pcSegment->line.sx, pcSegment->line.sy,
									 pcSegment->line.ex, pcSegment->line.ey);
		}
	}

	for (i = 0; i < CurrentSketch->Strokes.GetSize(); i++) {
		for (int j = 0; j < CurrentSketch->Strokes[i]->seglist.GetSize(); j++) {
			Segment *pcSegment = CurrentSketch->Strokes[i]->seglist[j];
			// Convert to MySegment
			MySegment cMySegment(pcSegment->line.sx, pcSegment->line.sy,
								 pcSegment->line.ex, pcSegment->line.ey,
								 pcSegment->arc.cx, pcSegment->arc.cy,
								 pcSegment->arc.rad,
								 pcSegment->arc.real_sang,
								 pcSegment->arc.real_eang,
								 pcSegment->arc.dir == 1 ? CCW : CW,
								 pcSegment->type == LINE_SEG ? LINE : ARC);
			switch (pcSegment->type) {
			case LINE_SEG:
				if (cMySegment.PointOnLine(Coord(x, y))) {
					printf("%d: (%.04f,%.04f)-(%.04f,%.04f), RL=%.04f\n",
						   iCount,
						   pcSegment->line.sx, pcSegment->line.sy,
						   pcSegment->line.ex, pcSegment->line.ey,
						   distance(pcSegment->line.sx, pcSegment->line.sy,
									pcSegment->line.ex, pcSegment->line.ey) /
						   dTotalLength);
				}
				break;
			case ARC_SEG:
				break;
			default:
				break;
			}

			iCount++;
		}
	}
}

bool glue_sketch_empty(RawSketch *pcSketch) {
	assert(pcSketch != NULL);
	if (pcSketch->Strokes.GetSize() == 0) {
		return (true);
	} else {
		return (false);
	}
}

bool glue_sketches_empty(SketchList *pcSketches) {
	assert(pcSketches != NULL);

	if (pcSketches->GetSize() == 1 && glue_sketch_empty((*pcSketches)[0])) {
		return (true);
	} else {
		return (false);
	}
}

const char *glue_sketches_title(RawSketch *pcSketch, SketchList *pcSketches) {
	static char pzcTitle[128];

	assert(pcSketch != NULL);
	assert(pcSketches != NULL);

	int i, iTotal = pcSketches->GetSize();
	// Look for position of current sketch
	for (i = 0; i < iTotal; i++) {
		if (pcSketch == (*pcSketches)[i]) {
			break;
		}
	}

	// Format the title
	if (i < iTotal) {
		snprintf(pzcTitle, sizeof(pzcTitle), "%d/%d", i + 1, iTotal);
	} else {
		snprintf(pzcTitle, sizeof(pzcTitle), "[unknown]/%d", iTotal);
	}

	return (pzcTitle);
}

void glue_compare(SketchList *pcSketches) {
	if (pcSketches->GetSize() != 2) {
		DEBUG("We need only 2 sketches to compare, not %d!",
			pcSketches->GetSize());
		return;
	}

	Symbols cSymbols;
	for (int i = 0; i < pcSketches->GetSize(); i++) {
		Symbol *pcSymbol = glue_new_symbol((*pcSketches)[i]);
		if (pcSymbol) {
			cSymbols.push_back(pcSymbol);
		}
	}

	cSymbols.compare();
	cSymbols.display();
}

void glue_answer(SketchList *pcSketches) {
	int i, j, k;
	Segment *seg;
	Circle_Dir arcdir;
	double arcSang,arcEang;
	MySegment* segment = NULL;
	Symbol* symbol= NULL;
	Symbols cSymbols;

	for (i = 0; i < pcSketches->GetSize(); i++) {
		RawSketch *pcSketch = (*pcSketches)[i];
		for(j=0; j<pcSketch->Strokes.GetSize(); j++) {
			for(k=0; k<pcSketch->Strokes[j]->seglist.GetSize(); k++) {
				seg = pcSketch->Strokes[j]->seglist[k];

				if (seg->arc.dir == 1) {
					arcdir=CCW;
				} else {
					arcdir=CW; 
				}

				arcSang=seg->arc.real_sang;
				arcEang=seg->arc.real_eang;
				assert(arcSang>=0.0 && arcEang>=0.0 && arcSang<=2*PI && arcEang<=2*PI);

				segment = new MySegment(seg->line.sx, seg->line.sy,
										seg->line.ex, seg->line.ey,
										seg->arc.cx, seg->arc.cy,
										seg->arc.rad, arcSang, arcEang, arcdir,
										seg->type == LINE_SEG ? LINE : ARC);
				assert(segment != NULL);
				if (symbol == NULL) {
					symbol = new Symbol;
					assert(symbol != NULL);
					cSymbols.push_back(symbol);
					symbol->Add_Segment(segment);
				} else {
//					if (SEGMENT_X_OVERLAPPED((*symbol)[symbol->size() - 1],
//									         segment)) {
					if (symbol->xOverlapped(segment)) {
						symbol->Add_Segment(segment);
					} else {
						symbol = new Symbol;
						assert(symbol != NULL);
						cSymbols.push_back(symbol);
						symbol->Add_Segment(segment);
					}
				}
			}
		}
	}

	// Process all the symbols
	for (i = 0; i < cSymbols.size(); i++) {
		cSymbols[i]->Process();
	}

	//printf("# of symbols = %d\n", cSymbols.size());

	string cExpr;

	for (i = 0; i < cSymbols.size(); i++) {
		Symbol *pcSymbol = cSymbols[i];

		char pzcMatch[256];
		pzcMatch[0] = 0;

		Recognizer cRecognizer;
		cRecognizer.recognize(pcSymbol, "Test symbol",
							  Match::GREEDY, GLUE_N_BEST, NULL, 0,
							  pzcMatch);
		printf("%d: %s\n", i, pzcMatch);
		if (pzcMatch[0]) {
			switch (pzcMatch[0]) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				cExpr += pzcMatch[0];
				break;
			case 'a':
				cExpr += " + ";
				break;
			case 's':
				cExpr += " - ";
				break;
			case 'm':
				cExpr += " * ";
				break;
			case 'd':
				cExpr += " / ";
				break;
			case 'e':
				cExpr += " = ";
				break;
			default:
				fprintf(stderr, "Unknown symbol: %c\n", pzcMatch[0]);
				break;
			}
		}
	}

	Infix2Postfix cInfix2Postfix(cExpr.c_str());
	double dResult = cInfix2Postfix.eval();
	char pzcResult[256];
	if (cInfix2Postfix.ok()) {
		snprintf(pzcResult, sizeof(pzcResult), "%.01f", dResult);
	} else {
		snprintf(pzcResult, sizeof(pzcResult), "you idiot");
	}

	printf("Expr: %s, ans = %s\n", cExpr.c_str(), pzcResult);
	Sound(pzcResult).play();
}
