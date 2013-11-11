// $Id: extern.h,v 1.3 2005/12/03 19:10:33 weesan Exp $

/*
 * extern.h - A hack to reference some global variables from
 *            Prof. Stahovich's segmentation code
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	5/10/2005
 */

#ifndef EXTERN_H
#define EXTERN_H

#include "ink.h"

extern int show_ink;
extern int show_seg;
extern int show_seg_ends;
extern int show_seg_point;
extern int StartNewStroke;
extern SketchList Sketches;
extern Stroke *stroke_data;
extern RawSketch *CurrentSketch;
extern double ParamSpeedThresh;

#endif // EXTERN_H