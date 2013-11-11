// $Id: symbol.cpp,v 1.44 2006/12/27 22:25:26 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#include "stdafx.h"
#include "symbol.h"
// Added by weesan@cs.ucr.edu
#include "definition.h"
#include "dist_prob.h"
#include "glue.h"
#include "distance.h"
#include "symbol_viewer.h"
#include "debug.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
// Added by weesan@cs.ucr.edu
#include <algorithm>
#include <fstream>
#include <vector>

// Added by weesan@cs.ucr.edu
#define SYMBOL_SKIP_LINK

#define BETW(x, u, v)			\
	(((u) < (v)) ?				\
	(x) >= (u) && (x) <= (v) :	\
	(x) >= (v) && (x) <= (u))

#define SYMBOL_SWAP_FAIL_SAFE	20

// XXX: using namespace is not working here :(
using namespace std;

// The comparison algorithm is taken from:
// Hierarchical Attributed Graph Representation and Recognition of Handwritten
// Chinese Characters, S. Lu, Y. Ren and C Suen, 1991
/*
static bool segmentCmp(const MySegment *s1, const MySegment *s2) {
	if (s1->Type() == LINE && s2->Type() == LINE) {
		// Rule 1a: One segment is on the left of another, non-overlapping
		if (!BETW(s1->StartPoint().x, s2->StartPoint().x, s2->EndPoint().x) &&
			!BETW(s1->EndPoint().x, s2->StartPoint().x, s2->EndPoint().x)) {
			//printf("Rule 1a applied\n");
			if (max(s1->StartPoint().x, s1->EndPoint().x) <=
				min(s2->StartPoint().x, s2->EndPoint().x)) {
				return (true);
			} else {
				return (false);
			}
		}
		
		// Rule 1b: One segment is on the top of another, non-overlapping
		if (!BETW(s1->StartPoint().y,
				  s2->StartPoint().y, s2->EndPoint().y) &&
			!BETW(s1->EndPoint().y,
				  s2->StartPoint().y, s2->EndPoint().y)) {
			//printf("Rule 1b applied\n");
			if (min(s1->StartPoint().y, s1->EndPoint().y) >=
				max(s2->StartPoint().y, s2->EndPoint().y)) {
				return (true);
			} else {
				return (false);
			}
		}

		// Rule 2: One segments on top of another but does not intersect
		double A1 = 0, B1 = 0, C1 = 0, A2 = 0, B2 = 0, C2 = 0;
		line_equation(s1->StartPoint(), s1->EndPoint(), &A1, &B1, &C1);
		line_equation(s2->StartPoint(), s2->EndPoint(), &A2, &B2, &C2);
	
		double l1 = A1 * s2->StartPoint().x +
					B1 * s2->StartPoint().y +
					C1;
		double l2 = A1 * s2->EndPoint().x +
					B1 * s2->EndPoint().y +
					C1;
		double l3 = A2 * s1->StartPoint().x +
					B2 * s1->StartPoint().y +
					C2;
		double l4 = A2 * s1->EndPoint().x +
					B2 * s1->EndPoint().y +
					C2;
		if (l1 * l2 < 0) {
			if (l3 > 0 && l4 > 0) {
				//printf("Rule 2a applied\n");
				return (true);
			} else if (l3 < 0 && l4 < 0) {
				//printf("Rule 2b applied\n");
				return (false);
			}
		}

		// Rule 3: Have to check if 2 segments intersect
		Coord intersect;
		if (LineSegLineSeg_intersect(s1->StartPoint(), s1->EndPoint(),
									 s2->StartPoint(), s2->EndPoint(),
									 intersect)) {
			double a1 = atan2(s1->StartPoint().y - s1->EndPoint().y,
							  s1->StartPoint().x - s1->EndPoint().x);
			double a2 = atan2(s2->StartPoint().y - s2->EndPoint().y,
							  s2->StartPoint().x - s2->EndPoint().x);
			// atan2() returns [-PI/2, PI/2], so, we need to convert negative
			// values into positive by adding PI so that we have [0, PI]
			if (a1 < 0) {
				a1 += PI;
			}
			if (a2 < 0) {
				a2 += PI;
			}
			assert(a1 >= 0 && a2 >= 0);

			if (a1 < a2) {
				//printf("Rule 3a applied\n");
				return (true);
			} else {
				//printf("Rule 3b applied\n");
				return (false);
			}
		}

		// Rule 4: XXX
		if (l3 > 0 && l4 > 0) {
			//printf("Rule 4a applied\n");
			return (true);
		} else {
			//printf("Rule 4b applied\n");
			return (false);
		}
	} else if (s1->Type() == LINE && s2->Type() == ARC) {
		Coord &p1 = min(s1->StartPoint(), s1->EndPoint());
		return (p1 < s2->Center());
	} else if (s1->Type() == ARC && s2->Type() == LINE) {
		Coord &p2 = min(s2->StartPoint(), s2->EndPoint());
		return (s1->Center() < p2);
	} else {
		return (s1->Center() < s2->Center());
	}
}
*/
/*
static bool segmentCmp(const MySegment *s1, const MySegment *s2) {
	return (s1->score() < s2->score());
}
*/

static bool segmentCmp(const MySegment *s1, const MySegment *s2) {
	if (s1->Type() == LINE && s2->Type() == LINE) {
		Coord &p1 = min(s1->StartPoint(), s1->EndPoint());
		Coord &p2 = min(s2->StartPoint(), s2->EndPoint());
		return (p1 < p2);
	} else if (s1->Type() == LINE && s2->Type() == ARC) {
		Coord &p1 = min(s1->StartPoint(), s1->EndPoint());
		return (p1 < s2->Center());
	} else if (s1->Type() == ARC && s2->Type() == LINE) {
		Coord &p2 = min(s2->StartPoint(), s2->EndPoint());
		return (s1->Center() < p2);
	} else {
		return (s1->Center() < s2->Center());
	}
}


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

Symbol::Symbol() : 
	Object("Symbol"),
	iMinX(INT_MAX),
	iMinY(INT_MAX),
	iMaxX(0),
	iMaxY(0),
	dTotalLength(0) {
}

Symbol::Symbol(const Symbol &s) :
	Object("Symbol"),
	iMinX(INT_MAX),
	iMinY(INT_MAX),
	iMaxX(0),
	iMaxY(0),
	dTotalLength(0) {
	*this = s;
	for (int i = 0; i < s.size(); i++) {
		(*this)[i] = new MySegment(*s[i]);
	}
	Process();
	/*
	for (int i = 0; i < s.size(); i++) {
		push_back(new MySegment(*s[i]));
	}
	*/
}

void Symbol::Add_Segment(MySegment* S)
{
	assert(S!=NULL);
	push_back(S->segNo(size()));
	segindexmap.push_back(size()-1);

	assert(size()<=MAX_NUMOF_SEGMENTS_INA_SYMBOL);

	// Added by weesan@cs.ucr.edu
	switch (S->Type()) {
	case LINE:
		iMinX = min(iMinX, min((int)S->StartPoint().x, (int)S->EndPoint().x));
		iMinY = min(iMinY, min((int)S->StartPoint().y, (int)S->EndPoint().y));
		iMaxX = max(iMaxX, max((int)S->StartPoint().x, (int)S->EndPoint().x));
		iMaxY = max(iMaxY, max((int)S->StartPoint().y, (int)S->EndPoint().y));
		break;
	case ARC: {
		vector<double> x, y;

		// Push back coord of begin and end of the segment
		x.push_back(S->Center().x + S->Radius() * cos(S->SmallAngle()));
		x.push_back(S->Center().x + S->Radius() * cos(S->BigAngle()));
		y.push_back(S->Center().y + S->Radius() * sin(S->SmallAngle()));
		y.push_back(S->Center().y + S->Radius() * sin(S->BigAngle()));
		// Push back other coords that fall at 90, 180, 270, 360, ...
		for (double a = 0; a < S->BigAngle(); a += PI / 2) {
			if (a > S->SmallAngle()) {
				x.push_back(S->Center().x + S->Radius() * cos(a));
				y.push_back(S->Center().y + S->Radius() * sin(a));
			}
		}

		// Pick the min and max values
		iMinX = min(iMinX, (int)*min_element(x.begin(), x.end()) + 0.5);
		iMinY = min(iMinY, (int)*min_element(y.begin(), y.end()) + 0.5);
		iMaxX = max(iMaxX, (int)*max_element(x.begin(), x.end()) + 0.5);
		iMaxY = max(iMaxY, (int)*max_element(y.begin(), y.end()) + 0.5);
		break;
	}
	default:
		break;
	}
}

void Symbol::Process()
{
	int i;

#ifdef DEF_LOC_WEIGHT
	// Added by weesan@cs.ucr.edu
	// Compute the location bitmap of each segment against 4x4 grid
	int x1 = minX();
	int y1 = minY();
	int x2 = maxX();
	int y2 = maxY();
	int xSteps = (x2 - x1 + SYMBOL_CHUCK_SIZE - 1) / SYMBOL_CHUCK_SIZE;
	int ySteps = (y2 - y1 + SYMBOL_CHUCK_SIZE - 1) / SYMBOL_CHUCK_SIZE;

	for (i = 0; i < size(); i++) {
		MySegment *pcSegment = (*this)[i];
		Coord e1 = Coord(pcSegment->StartPoint().x, pcSegment->StartPoint().y);
		Coord e2 = Coord(pcSegment->EndPoint().x, pcSegment->EndPoint().y);
		Coord c;
		unsigned int uiLocBitmap = 0;
		int iBit = SYMBOL_CHUCK_SIZE * SYMBOL_CHUCK_SIZE - 1;
		for (int y = y1; y < y2; y += ySteps) {
			for (int x = x1; x < x2; x += xSteps) {
				int iTop = LineSegLineSeg_intersect(
					e1, e2,
					Coord(x, y),
					Coord(x + xSteps, y),
					c);
				int iLeft = LineSegLineSeg_intersect(
					e1, e2,
					Coord(x, y),
					Coord(x, y + ySteps),
					c);
				int iRight = LineSegLineSeg_intersect(
					e1, e2,
					Coord(x + xSteps, y),
					Coord(x + xSteps, y + ySteps),
					c);
				int iBottom = LineSegLineSeg_intersect(
					e1, e2,
					Coord(x, y + ySteps),
					Coord(x + xSteps, y + ySteps),
					c);
				if (iTop || iLeft || iRight || iBottom) {
					uiLocBitmap |= 0x01 << iBit;
				}
			iBit--;
			}
		}
		pcSegment->locBitmap(uiLocBitmap);
	}
#endif 

	// Compute the total length of the symbol
	dTotalLength = 0.0;
	for (i = 0; i < size(); i++) {
		dTotalLength += (*this)[i]->Length();
	}

	RelTable.clear_table();
	for(i=0;i<size();i++) {	
		// Set the intrinsic prop, eg. relative length of each segment
		RelTable.set(IntrinsicProp((*this)[i],
								   totalLength(),
								   (*this)[i]->locBitmap()),
					 i);
		for(int j=i+1;j<size();j++) {
			/* 
			 * Set the relationship among each segment,
			 * eg. # of intersection, intersection angle, intersection locations
			 */
			RelTable.set((*this)[i],(*this)[j],i,j);
		}
	}

	// Added by weesan@cs.ucr.edu
#ifdef DEF_CENTROID_WEIGHT
	computeCentroid();
#endif
}

void Symbol::Shuffle_Segments(int iDefSize)
{
	int iMax = size();
	//int iMax = max(size(), iDefSize);

	if (size() < iMax) {
		segindexmap.resize(iMax, -1);
	}

	int a,b,c,temp;
	a=rand() % iMax;
	b=rand() % iMax;
	c=rand() % iMax;

	temp=segindexmap[a];
	segindexmap[a]=segindexmap[b];
	segindexmap[b]=segindexmap[c];
	segindexmap[c]=temp;
}

const char *Symbol::drawOrder(void) const {
	static char pzcDrawOrder[2][256];
	static int iIndex = 0;

	iIndex = (iIndex + 1) % 2;
	char *p = pzcDrawOrder[iIndex];
	int iLen = 0;

	p[0] = 0;
	for (int i = 0; i < segindexmap.size(); i++) {
		char pzcSegNo[16];
		int iSegNo = segindexmap[i];
		if (iSegNo >= 0) {
			snprintf(pzcSegNo, sizeof(pzcSegNo), "%d", iSegNo);
		} else {
			pzcSegNo[0] = '?';
			pzcSegNo[1] = 0;
		}
		if (i == 0) {
			iLen += snprintf(p + iLen, sizeof(pzcDrawOrder[0]) - iLen,
						     "%s", pzcSegNo);
		} else {
			iLen += snprintf(p + iLen, sizeof(pzcDrawOrder[0]) - iLen,
						     "-%s", pzcSegNo);
		}
	}

	return (p);
}

MySegment *Symbol::getSegment(int i) const {
	assert(i >= 0 && i < size());
	int iIndex = segindexmap[i];
	if (iIndex >= 0 && iIndex < size()) {
		return ((*this)[iIndex]);
	} else {
		return (NULL);
	}
}

Relationship* Symbol::getRelation(int indexA,int indexB, bool *pbSwap) const {
	if (indexA < size() && indexB < size() && indexA >= 0 && indexB >= 0) {
		assert(indexA != indexB);

		int a = segindexmap[indexA];
		int b = segindexmap[indexB];

		if(a>b) {
			if (pbSwap) {
				*pbSwap = true;
			}
			return (RelTable.get(b,a));
		} else {
			if (pbSwap) {
				*pbSwap = false;
			}
			return (RelTable.get(a,b));
		}
	} else {
		return (NULL);
	}
}

IntrinsicProp* Symbol::getIP(int indexA) const {
	if (indexA >= 0 && indexA < size()) {
		return (RelTable.get(segindexmap[indexA]));
	} else {
		return (NULL);
	}
}

// Added by weesan@cs.ucr.edu
void Symbol::sort(void) {
/*
	int x1 = minX();
	int y1 = minY();
	int x2 = maxX();
	int y2 = maxY();

	for (int i = 0; i < size(); i++) {
		unsigned int uiScore = 0;
		MySegment *pcSegment = (*this)[i];
		Coord e1 = Coord(pcSegment->StartPoint().x, pcSegment->StartPoint().y);
		Coord e2 = Coord(pcSegment->EndPoint().x, pcSegment->EndPoint().y);
		Coord c;
		int iWeight = 1;
		int iTotalWeight = 1;
		for (int y = y1; y < y2; y += SYMBOL_CHUCK_SIZE) {
			for (int x = x1; x < x2; x += SYMBOL_CHUCK_SIZE) {
				int iTop = LineSegLineSeg_intersect(
					e1, e2,
					Coord(x, y),
					Coord(x + SYMBOL_CHUCK_SIZE, y),
					c);
				int iLeft = LineSegLineSeg_intersect(
					e1, e2,
					Coord(x, y),
					Coord(x, y + SYMBOL_CHUCK_SIZE),
					c);
				int iRight = LineSegLineSeg_intersect(
					e1, e2,
					Coord(x + SYMBOL_CHUCK_SIZE, y),
					Coord(x + SYMBOL_CHUCK_SIZE, y + SYMBOL_CHUCK_SIZE),
					c);
				int iBottom = LineSegLineSeg_intersect(
					e1, e2,
					Coord(x, y + SYMBOL_CHUCK_SIZE),
					Coord(x + SYMBOL_CHUCK_SIZE, y + SYMBOL_CHUCK_SIZE),
					c);
				if (iTop || iLeft || iRight || iBottom) {
					uiScore += iWeight;
				}

				iWeight++;
				iTotalWeight += iWeight;
			}
			iWeight = iTotalWeight + 1;
		}
		pcSegment->score(uiScore);
	}
*/
	// Sort the segments
	std::sort(begin(), end(), segmentCmp);

	// Recreate the relationship table
	Process();

	// Reset segindexmap and bestdraworder
	segindexmap.clear();
	clearBestDrawOrder();

	// Recreate segindexmap
	for (int i = 0; i < size(); i++) {
		segindexmap.push_back(i);
	}
}

void Symbol::assignDrawOrder(const char *pzcAssignedOrder) {
	const char pzcDelim[] = " -";

	if (!pzcAssignedOrder) {
		return;
	}

	// Make a tmp copy
	char *pzcBuf = new char[strlen(pzcAssignedOrder) + 1];
	assert(pzcBuf != NULL);
	strcpy(pzcBuf, pzcAssignedOrder);

	// Reset segindexmap
	segindexmap.clear();

	for (const char *p = strtok(pzcBuf, pzcDelim);
	     p;
		 p = strtok(NULL, pzcDelim)) {
		segindexmap.push_back(atoi(p));
	}

	// Free the tmp copy
	delete []pzcBuf;

	if (segindexmap.size() != size()) {
		fprintf(stderr,
			    "Warning: Failed to assign order (%s) "
				"as it has only %d segments\n",
				pzcAssignedOrder, size());
		restoreOrigOrder();
	}
}

void Symbol::assignDrawOrder(const vector<int> &cAssignedOrder) {
	// Clear the existing indexmap
	segindexmap.clear();

	// Allocate new set of index
	segindexmap.resize(cAssignedOrder.size(), -1);

	// Assign new ones
	for (int i = 0; i < cAssignedOrder.size(); i++) {
		int iIndex = cAssignedOrder[i];
		// If index of the vector is not big enuf, resize it
		// NOTE: This might happen when the unknown symbol has lesser
		// number of segments and one of them is matched to higher number of
		// segment in the definition
		// NOTE2: iIndex could be -1 due to unknown has lesser number of
		// segments
		if (iIndex < 0) {
			continue;
		} else if (iIndex >= segindexmap.size()) {
			segindexmap.resize(iIndex + 1, -1);
		}
		segindexmap[iIndex] = i;
	}
}


class LocalError {
public:
	double primType;
	double relLen;
	double intNo;
	double intAng;
	double intLoc;
	double conn;
public:
	LocalError(void) :
		primType(0),
		relLen(0),
		intNo(0),
		intAng(0),
		intLoc(0),
		conn(0) {
	}
	void set(double v) {
		primType = relLen = intNo = intAng = intLoc = conn = v;
	}
};

/*
 * Unlike Definition::PercentMatch(), which computes the global
 * error matrics, this computes only the local error metrics, so,
 * some error matrics might not be applicable here
 */
void Symbol::computeLocalError(Definition *def) {
	vector<LocalError> errors(size());
	int i, j;
	int segmentNo = min(size(), def->NumofSegments());

	// Give error of 1.0 for extra segments
	if (def->NumofSegments() < size()) {
		for (i = def->NumofSegments(); i < size(); i++) {
			errors[i].set(1.0);
		}
	}

	// Reset the local error before computing one
	localError.clear();

	// 1) Prim Type Error
	for (i = 0; i < segmentNo; i++) {
		if(getIP(i)->SType() != def->getIP(i)->SType()) {
			errors[i].primType = 1.0;
		}
	}

	// 2) Prim Count Error - N/A

	// 3) Relative Length Error
	for (i = 0; i < segmentNo; i++) {
		const IntrinsicProp *dip = def->getIP(i);
		const IntrinsicProp *sip = getIP(i);
		errors[i].relLen = 1.0 - normalized_gauss_prob(dip->RelLen(),
													   dip->RelLen_stdev(),
													   sip->RelLen());
	}

	// 4) Number of Intersection Error
	for (i = 0; i < segmentNo; i++) {
		for (j = i + 1; j < segmentNo; j++) {
			Relationship *dr = def->getRelation(i,j);
			Relationship *sr = getRelation(i,j);
			errors[i].intNo += fabs((double)dr->NumofIntersections() -
									(double)sr->NumofIntersections());
		}
		int totalRel = segmentNo - i - 1;
		if (totalRel != 0) {
			errors[i].intNo /= totalRel;
		}
		errors[i].intNo = 1.0 / (1.0 + exp(6.0 * (1 - errors[i].intNo)));
	}

	// 5) Intersection Angle Error
	for (i = 0; i < segmentNo; i++) {
		int BothLinesCount=0;
		for (j = i + 1; j < segmentNo; j++) {
			Relationship *dr = def->getRelation(i,j);
			Relationship *sr = getRelation(i,j);
			if (dr->BothAreLines() && sr->BothAreLines()) {
				BothLinesCount++;
				errors[i].intAng += 1.0 - normalized_gauss_prob(
												dr->IntAngle(),
												dr->IntAngle_stdev(),
												sr->IntAngle());
			}
		}
		if (BothLinesCount != 0) {
			errors[i].intAng /= (double)BothLinesCount;
		}
	}

	// 6) Intersection Location Error
	for (i = 0; i < segmentNo; i++) {
		int IntlocationCount=0;
		for (j = i + 1; j < segmentNo; j++) {
			Relationship *dr = def->getRelation(i,j);
			Relationship *sr = getRelation(i,j);
			if (dr->NumofIntersections() && sr->NumofIntersections()) {
				if (dr->NumofIntersections() == 1) {
					IntlocationCount += 2; //add 2 because you are considering both A1 and B1, hence for normalization count 2 terms
					errors[i].intLoc += 1.0 - normalized_gauss_prob(dr->IntLocA1(),dr->IntLocA1_stdev(),sr->IntLocA1());
					errors[i].intLoc += 1.0 - normalized_gauss_prob(dr->IntLocB1(),dr->IntLocB1_stdev(),sr->IntLocB1());
				}
				if (dr->NumofIntersections()==2) {
					IntlocationCount += 4; //add 4 because you are considering both A1,B1,A2,B2, hence for normalization count 4 terms
					errors[i].intLoc += 1.0 - normalized_gauss_prob(dr->IntLocA1(),dr->IntLocA1_stdev(),sr->IntLocA1());
					errors[i].intLoc += 1.0 - normalized_gauss_prob(dr->IntLocB1(),dr->IntLocB1_stdev(),sr->IntLocB1());
					errors[i].intLoc += 1.0 - normalized_gauss_prob(dr->IntLocA2(),dr->IntLocA2_stdev(),sr->IntLocA2());
					errors[i].intLoc += 1.0 - normalized_gauss_prob(dr->IntLocB2(),dr->IntLocB2_stdev(),sr->IntLocB2());
				}
			} else if (dr->NumofIntersections() || sr->NumofIntersections()) {
				int iNoofInt = max(dr->NumofIntersections(),
								   sr->NumofIntersections());
				int iPenalty = iNoofInt * 2;
				IntlocationCount += iPenalty;
				errors[i].intLoc += iPenalty;
			}
		} 
		if (IntlocationCount != 0) {
			errors[i].intLoc /= (double)IntlocationCount;
		}
	}

#ifdef DEF_CONN_WEIGHT
	// 7) Connectivity for each segment with other segments
	double dConnError = 0;
	for (i = 0; i < segmentNo; i++) {
		int dConn = 0, sConn = 0;
		// NOTE: j = 0 not j = i + 1 as previous ones because we need the
		// intersection number from one segment to any others
		for (j = 0; j < segmentNo; j++) {
			if (i == j) {
				continue;
			} else {
				int a = i, b = j;
				if (a > b) {
					int t = a;
					a = b;
					b = t;
				}
				Relationship *dr = def->getRelation(a, b);
				Relationship *sr = getRelation(a, b);
				dConn += dr->NumofIntersections();
				sConn += sr->NumofIntersections();
			}
		}
		errors[i].conn += abs(dConn - sConn);
		// Average out by the number of segments
		errors[i].conn /= segmentNo;
		errors[i].conn = 1.0 / (1.0 + exp(6.0 * (1 - errors[i].conn)));
	}
#endif

	// Combine all errors
	for (i = 0; i < segmentNo; i++) {
		double e =
			errors[i].primType * DEF_PRIM_TYPE_WEIGHT +
			errors[i].relLen   * DEF_REL_LEN_WEIGHT +
			errors[i].intNo    * DEF_INT_NO_WEIGHT +
			errors[i].intAng   * DEF_INT_ANG_WEIGHT +
			errors[i].intLoc   * DEF_INT_LOC_WEIGHT
#ifdef DEF_CONN_WEIGHT
			+ errors[i].conn     * DEF_CONN_WEIGHT
#endif
			;
		localError.push_back((float)e);
		/*
		printf("Local error %d: %f (%f, %f, %f, %f, %f, %f)\n",
			i, localError[i],
			errors[i].primType, errors[i].relLen,
			errors[i].intNo, errors[i].intAng,
			errors[i].intLoc, errors[i].conn);
		*/
	}
}

// Return true only when 2 segments are swapped
bool Symbol::swapSegmentsBasedOnLocalError(void) {
	// It only makes sense when there are more than 2 segments
	if (size() <= 2) {
		return (false);
	}

	DistProb cDistProb(localError);
	int s1 = cDistProb.generate();
	int s2 = s1;

	int iFailSafe = 0;
	while (s2 == s1) {
		if (iFailSafe++ >= SYMBOL_SWAP_FAIL_SAFE) {
			//printf("Warning: take too long to converge!\n");
			break;
		}
		s2 = cDistProb.generate();
	}

	// Swap 2 segments
	//printf("Swapping segs %d and %d\n", s1, s2);
	if (s1 != s2) {
		swapSegments(s1, s2);
		return (true);
	} else {
		return (false);
	}
}

void Symbol::write(std::ostream &os) const {
	CTime tNow = CTime::GetCurrentTime();
	os << endl
	   << "# Created on " << (LPCTSTR)tNow.Format("%B %d, %Y  %H:%M:%S") << endl;
	os << name() << " = {" << endl;
	for (int i = 0; i < size(); i++) {
		os << *(*this)[i] << endl;
	}
	os << "}" << endl;
}

void Symbol::read(std::istream &is) {
	if (!is) {
		return;
	}

	// Symbol = {
	char buf[256];
	while (is.getline(buf, sizeof(buf))) {
		if (strncmp(buf, "Symbol", 6) == 0) {
			break;
		}
	}

	while (is && is.peek() != '}') {
		MySegment *pcSegment = new MySegment;
		is >> *pcSegment;
		Add_Segment(pcSegment);
		//printf("  Read segment %d\n", size());
 	}

	// }
	is.getline(buf, sizeof(buf));
}

#ifdef DEF_CENTROID_WEIGHT
void Symbol::computeCentroid(void) {
	int iCount = 0;

	for (int i = 0; i < size(); i++) {
		MySegment *pcSegment = (*this)[i];
		switch (pcSegment->Type()) {
		case LINE:
			iCount++;
			cCentroid.x += (pcSegment->StartPoint().x - minX());
			cCentroid.x += (pcSegment->EndPoint().x - minX());
			cCentroid.y += (pcSegment->StartPoint().y - minY());
			cCentroid.y += (pcSegment->EndPoint().y - minY());
			break;
/*
		// Do not consider ARC for now since its center could be way off
		// the graph if the arc is close to a line
		case ARC:
			iCount++;
			cCentroid.x += (pcSegment->Center().x - minX());
			cCentroid.x += (pcSegment->Center().x - minX());
			break;
*/
		default:
			break;
		}
	}

	if (iCount > 0) {
		cCentroid.x /= iCount;
		cCentroid.y /= iCount;
	} else {
		cCentroid.set(0, 0);
	}
}
#endif

void Symbol::center(int *x, int *y) {
	assert(x != NULL && y != NULL);

	*x = (minX() + maxX()) / 2;
	*y = (minY() + maxY()) / 2;
}

void Symbol::flipSegment(int i) {
	MySegment *s = getSegment(i);
	assert(s != NULL);
	s->flip();
}

// Flip segments based on the their coordinates
void Symbol::flipSegments(void) {
	for (int i = 0; i < size(); i++) {
		MySegment *S = (*this)[i];
		/*
		// Swap it based on x-coord first then y-coord
		if (S->StartPoint().x > S->EndPoint().x) {
			S->flip();
		} else if (S->StartPoint().x == S->EndPoint().x) {
			if (S->StartPoint().y > S->EndPoint().y) {
				S->flip();
			}
		}
		*/
		// Swap the coords based on the importance of x or y axis
		// Doesn't work well on with lines with gradient almost 1 or -1
		double xDiff = fabs(S->StartPoint().x - S->EndPoint().x);
		double yDiff = fabs(S->StartPoint().y - S->EndPoint().y);
		if (xDiff > yDiff) {
			// Flip the segment based on X coordinates
			if (S->StartPoint().x > S->EndPoint().x) {
				S->flip();
			}
		} else {
			// Flip the segment based on Y coordinates
			if (S->StartPoint().y > S->EndPoint().y) {
				S->flip();
			}
		}
	}
}

bool Symbol::xOverlapped(MySegment *pcSegment) {
	for (int i = 0; i < size(); i++) {
		if (!SEGMENT_X_OVERLAPPED((*this)[i], pcSegment)) {
			return (false);
		}
	}

	return (true);
}

//////////////////////////////////

Symbols::Symbols(void) :
	ScanDir(NULL),
	iRandom(0) {
	pzcDir[0] = 0;
}

Symbols::Symbols(const char *_pzcDir, int _iRandom) :
	ScanDir(_pzcDir, "*.txt"),
	iRandom(_iRandom) {
	//pcIfs(NULL) {
	assert(_pzcDir != NULL);
	snprintf(pzcDir, sizeof(pzcDir), "%s", _pzcDir);
}

// Retrieve the name of the file without the extension
// NOTE: It's caller's responsibility to make sure 'pzcName' is big enuf
void Symbols::name(char *pzcName) {
	assert(pzcName != NULL);
	const char *p = filename();
	char *q = pzcName;
	while (p && *p != '.') {
		*q++ = *p++;
	}
	*q = 0;
}

bool Symbols::firstFile(void) {
	char pzcWildcard[_MAX_PATH + 1];
	snprintf(pzcWildcard, sizeof(pzcWildcard), "%s\\*.txt", dir());

	if (first()) {
		return (true);
	} else {
		return (false);
	}
}

bool Symbols::nextFile(void) {
	if (next()) {
		return (true);
	} else {
		return (false);
	}
}

Symbol *Symbols::firstSymbol(void) {
	// Get full path of the file
	char pzcFile[_MAX_PATH + 1];
	snprintf(pzcFile, sizeof(pzcFile), "%s\\%s", dir(), filename());

	// Return the processed symbol or NULL if it fails
	Symbol *pcSymbol = glue_first_symbol(pzcFile);
	if (random() && pcSymbol) {
		pcSymbol->randomize(random());
	}
	return (pcSymbol);
}

Symbol *Symbols::nextSymbol(void) {
	Symbol *pcSymbol = glue_next_symbol();
	if (random() && pcSymbol) {
		pcSymbol->randomize(random());
	}
	return (pcSymbol);
}

/*
 * Compute the distance of 2 symbols after scaling and resizing
 * the 2nd symbol against the 1st symbol
 */
void Symbols::compare(void) {
	if (size() != 2) {
		DEBUG("We need only 2 symbols to compare, not %d!", size());
		return;
	}

	Symbol *pcSymbol1 = (*this)[0];
	Symbol *pcSymbol2 = (*this)[1];
	assert(pcSymbol1 != NULL && pcSymbol2 != NULL);

	Distance(*pcSymbol2, *pcSymbol1, true);
}

void Symbols::display(void) {
#ifdef HAVE_FLTK
	SymbolsViewer cSymbolsViewer(this);
	cSymbolsViewer.run();
#else
	fprintf(stderr, "Warning: no FLTK!\n");
#endif
}

void Symbols::randomMark(int iCount) {
	if (iCount > size()) {
		fprintf(stderr, "Warning: nothing to mark here!\n");
		return;
	}
	
	bool bReversed = false;

	// Mark all symbol false
	resetMark();

	// If count is too many, instead of marking them all true,
	// we mark (size() - count) symbols true and reverse them later
	if (iCount > size() / 2) {
		bReversed = true;
		iCount = size() - iCount;
	}

	while (iCount) {
		int iIndex = rand() % size();
		//printf("%d\n", iIndex);
		if (!(*this)[iIndex]->mark()) {
			(*this)[iIndex]->mark(true);
			iCount--;
		} else {
			continue;
		}
	}

	if (bReversed) {
		reverseMark();
	}
}

//////////////////////////////////

TrainingSymbols::TrainingSymbols(const char *pzcUser, int iRandom, int iVerbose) {
	char cwd[_MAX_PATH + 1];
	char path[_MAX_PATH + 1];

	int iSize = GetCurrentDirectory(sizeof(cwd), cwd);
	assert(iSize != 0);

	printf("Reading all symbols ...\n");
	// Get symbols from definitions dir
	snprintf(path, sizeof(path), "%s\\%s\\definitions.%s",
			 cwd, DEF_DIR_PREFIX, pzcUser);
	scanDir(path, iRandom, iVerbose);
	// Get symbols from symbols dir
	snprintf(path, sizeof(path), "%s\\%s\\symbols.%s",
			 cwd, DEF_DIR_PREFIX, pzcUser);
	scanDir(path, iRandom, iVerbose);

	//printf("Average segment count / symbol = %.02f\n", aveSegmentCount());
}

void TrainingSymbols::scanDir(const char *pzcDir, int iRandom, int iVerbose) {
	Symbols cSymbols(pzcDir, iRandom);
	for (bool bRet = cSymbols.firstFile(); bRet; bRet = cSymbols.nextFile()) {
		// Retrieve the name of the definition without the extension
		char pzcName[_MAX_PATH + 1];
		cSymbols.name(pzcName);
#ifdef SYMBOL_SKIP_LINK
		if (strcmp(pzcName, "link") == 0) {
			continue;
		}
#endif
		if (iVerbose) {
			printf("%10s: ", pzcName);
		}
		for (Symbol *pcSymbol = cSymbols.firstSymbol();
			 pcSymbol;
			 pcSymbol = cSymbols.nextSymbol()) {
			Symbols &cSymbols = (*this)[pzcName];
			cSymbols.push_back(pcSymbol);
			if (iVerbose) {
				printf(".");
			}
		}
 	    if (iVerbose) {
			printf("\n");
		}
	}
}

void TrainingSymbols::randomMark(int iCount) {
	for (iterator itr = begin(); itr != end(); itr++) {
		Symbols &cSymbols = itr->second;
		cSymbols.randomMark(iCount);
		/*
		int iCount = 0;
		for (int i = 0; i < cSymbols.size(); i++) {
			if (cSymbols[i]->mark()) {
				iCount++;
			}
		}
		printf("%s: %d/%d\n", itr->first.c_str(), iCount, cSymbols.size());
		*/
	}
}

double TrainingSymbols::aveSegmentCount(void) {
	int iSegCount = 0;
	int iSymCount = 0;
	for (iterator itr = begin(); itr != end(); itr++) {
		Symbols &cSymbols = itr->second;
		iSymCount += cSymbols.size();
		for (int i = 0; i < cSymbols.size(); i++) {
			iSegCount += cSymbols[i]->size();
		}
	}

	return ((double)iSegCount / iSymCount);
}

void TrainingSymbols::dump(void) {
	for (iterator itr = begin(); itr != end(); itr++) {
		Symbols &cSymbols = itr->second;
		printf("Symbol = %s\n", itr->first.c_str());
		cSymbols.dump();
	}
}