// $Id: globals.h,v 1.3 2007/04/17 23:45:20 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#ifndef _GLOBALS_H
#define _GLOBALS_H
//created by Levent Burak Kara

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//math constants
#ifndef PI
#define PI (3.14159265)
#endif
#define INF (10e6)
#define EPSILON (10e-6)
#define EQUALITY_EPS (10e-4)
#define PARALLELISM_ANGLE_ZONE (3.0*PI/180.0)
#define INTERSECTION_TOLERANCE_PERCENTAGE (0.10)  //+-10% of the line length 

#define PARALLELISM_FREQ_THRESHOLD (0.5)
#define PERPENDICULARITY_FREQ_THRESHOLD (0.5)
#define INTERSECTION_FREQ_THRESHOLD (0.7)
#define INT_ANGLE_STDDEV_THRESHOLD (5.0*PI/180.0)
#define REL_INT_LOC_STDDEV_THRESHOLD (0.05)

#define MAX_NUMOF_SEGMENTS_INA_SYMBOL (20)



//MATCH ERROR CONSTANTS
#define PRIM_CNT_DIFF_WEIGHT (0.15)
#define PRIM_TYPE_WEIGHT (1.0)
#define MISSING_INTERSC_WEIGHT (1.0)
#define MISSING_PARALLELISM_WEIGHT (1.0)
#define MISSING_PERPENDC_WEIGHT (1.0)

#define ABS_LENGTH_E (1.0)
#define REL_LENGTH_E (1.0)
#define REL_LENGTH_R (100.0)
#define INT_LOC_E    (0.33)
#define INT_LOC_R    (100.0)
#define INT_ANGLE_E  (0.17)
#define INT_ANGLE_R  (180.0)

enum Segment_Type {LINE,ARC,UNKNOWN_TYPE};
enum Circle_Dir {CW,CCW,UNKNOWN_DIR};

//directory names
const CString _RAWDATA_="RawTrain\\";
const CString _DEFINITIONS_="definitions\\";


#endif