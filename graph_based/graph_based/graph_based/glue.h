// $Id: glue.h,v 1.11 2006/02/16 01:23:52 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/

#ifndef GLUE_H
#define GLUE_H

#include "stdafx.h"
#include "ink.h"
#include "symbol.h"
#include "training.h"

int glue_train(CView *pcView, SketchList *pcSketches);
int glue_recognize(RawSketch *pcSketch);
bool glue_write_raw_data(const char *pzcName);
bool glue_read_raw_data(const char *pzcName);
Symbol *glue_first_symbol(const char *pzcName);
Symbol *glue_next_symbol(void);
Symbol *glue_new_symbol(RawSketch *pcSketch);
#ifdef TRAINING_RAW
RawSymbol *glue_raw_symbol(const char *pzcName);
#endif
void glue_display_segment_no(void);
void glue_display_segment_coord(int x, int y);
bool glue_sketch_empty(RawSketch *pcSketch);
bool glue_sketches_empty(SketchList *pcSketches);
const char *glue_sketches_title(RawSketch *pcSketch, SketchList *pcSketches);
void glue_compare(SketchList *pcSketches);
void glue_answer(SketchList *pcSketches);

#endif // GLUE_H