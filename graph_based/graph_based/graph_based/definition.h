// $Id: definition.h,v 1.31 2006/12/23 08:37:39 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/

#ifndef DEFINITION_H
#define DEFINITION_H
//created by Levent Burak Kara

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "object.h"
#include "symbol.h"
#include "relationship.h"
#include "globals.h"
#include "match.h"
#include "population.h"
#include <vector>

using namespace std;

#define DEF_DIR_PREFIX			"data"
#define DEF_DIR					DEF_DIR_PREFIX "\\definitions"

#define DEF_PRIM_TYPE_WEIGHT	0.20
#define DEF_PRIM_COUNT_WEIGHT	0.20
#define DEF_REL_LEN_WEIGHT		0.20
#define DEF_INT_NO_WEIGHT		0.15
#define DEF_INT_ANG_WEIGHT		0.15
#define DEF_INT_LOC_WEIGHT		0.10
/*
#define DEF_PRIM_TYPE_WEIGHT	0.20
#define DEF_PRIM_COUNT_WEIGHT	0.15
#define DEF_REL_LEN_WEIGHT		0.15
#define DEF_INT_NO_WEIGHT		0.15
#define DEF_INT_ANG_WEIGHT		0.15
#define DEF_INT_LOC_WEIGHT		0.10
#define DEF_CONN_WEIGHT			0.10
*/
/*
#define DEF_PRIM_TYPE_WEIGHT	0.20
#define DEF_PRIM_COUNT_WEIGHT	0.15
#define DEF_REL_LEN_WEIGHT		0.15
#define DEF_INT_NO_WEIGHT		0.10
#define DEF_INT_ANG_WEIGHT		0.10
#define DEF_INT_LOC_WEIGHT		0.10
#define DEF_CENTROID_WEIGHT     0.20
*/
/*
#define DEF_PRIM_TYPE_WEIGHT	0.20
#define DEF_PRIM_COUNT_WEIGHT	0.15
#define DEF_REL_LEN_WEIGHT		0.10
#define DEF_INT_NO_WEIGHT		0.10
#define DEF_INT_ANG_WEIGHT		0.15
#define DEF_INT_LOC_WEIGHT		0.10
#define DEF_LOC_WEIGHT			0.20
*/

class Definition : public Object, public vector<Symbol *> {
private:
	int ValidSymbol(const Symbol* S);

protected:
	virtual void write(std::ostream &os) const;
	void read(std::istream &is) {
		// Nothing for now
	}

public:
	Definition(const char *pzcName = NULL);
	~Definition() {
		// Added by weesan@cs.ucr.edu
		for (int i = 0; i < size(); i++) {
			delete (*this)[i];
		}
	}
	int AddTrainSymbol(Symbol* S);
	void process(int iFirstN = 0);
	double PercentMatch(const Symbol* S,
						bool bTrainingMode = false,
						double dDeviation = 0.0,
						double dPrimTypeWeight = DEF_PRIM_TYPE_WEIGHT,
						double dPrimCountWeight = DEF_PRIM_COUNT_WEIGHT,
						double dRelLenWeight = DEF_REL_LEN_WEIGHT,
						double dIntNoWeight = DEF_INT_NO_WEIGHT,
						double dIntAngWeight = DEF_INT_ANG_WEIGHT,
						double dIntLocWeight = DEF_INT_LOC_WEIGHT
#ifdef DEF_CONN_WEIGHT
						,double dConnWeight = DEF_CONN_WEIGHT
#endif
#ifdef DEF_CENTROID_WEIGHT
						, double dCentroidWeight = DEF_CENTROID_WEIGHT					
#endif
#ifdef DEF_LOC_WEIGHT
						, double dLocWeight = DEF_LOC_WEIGHT
#endif
						) const;

	Relationship*  getRelation(int indexA,int indexB) const;
	IntrinsicProp*  getIP(int indexA) const;
	int NumofSegments() const;

	void print() const;
	CString name() const {
		return (cName);
	}
	void name(const char *pzcName) {
		cName = pzcName;
	}

	// Added by weesan@cs.ucr.edu
	void sortSymbols(void) {
		for (int i = 0; i < size(); i++) {
			(*this)[i]->sort();
		}
		// Have to re-process the symbols
		process();
	}
	void restoreOrigOrder(void) {
		for (int i = 0; i < size(); i++) {
			(*this)[i]->restoreOrigOrder();
		}
		// Have to re-process the symbols
		process();
	}
	Coord centroid(void) const {
		return (cCentroid);
	}
	void sort(void);

private:
	CString cName;
	int numof_segments;
	RelationTable RelTable;
	// Added by weesan@cs.ucr.edu
	Population cMeanNoofSegments;
	Coord cCentroid;
};

// Added by weesan@cs.ucr.edu
class Definitions : public vector<Definition *> {
public:
	enum Type {
		VANILLA,
		INCREMENT,
		FEEDBACK,
		BEST_N,
		DISTANCE
	};

private:
	char resultCode(int iResult);
	void addSymbol(Symbol *pcSymbol, Definition *pcDef,
				   Match::Type eMatch, bool bDistance,
				   int iVerbose = 1);

public:
	~Definitions(void) {
		for (int i = 0; i < size(); i++) {
			delete (*this)[i];
		}
	}
	bool write(const char *pzcOutDir);
	void read(const char *pzcDefDir);
	void readIncrement(const char *pzcDefDir, Match::Type eMatch,
					   int iRandom, bool bDistance = false);
	void feedback(Match::Type eMatch);
	void bestN(Match::Type eMatch, int n = 0);
	void readDeviation(const char *pzcDefDir);
	void dumpDrawOrder(void);
	void dumpIncStdDev(void);
	void dumpGraph(void);
	bool plotGraph(const char *pzcOutDir);
	void readTrainingSymbols(TrainingSymbols &cTrainingSymbols,
							 int iVerbose);
};

extern Definitions cDefs;

#endif