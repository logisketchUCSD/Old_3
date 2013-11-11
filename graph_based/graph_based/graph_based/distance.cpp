// $Id: distance.cpp,v 1.6 2006/12/23 08:37:40 weesan Exp $

/*
 * distance.cpp - Compute the distance between 2 segments (lines and arcs)
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/13/2005
 */

#include <assert.h>
#include "distance.h"
#include "vector.h"
#include "dist_prob.h"

#define DISTANCE_SAMPLE_RATE	50

#define DISTANCE_PROB_N			100
#define DISTANCE_SWAP_FAIL_SAFE	20

using namespace std;

static bool scoreCmp(const Distance::Score &s1, const Distance::Score &s2) {
	if (s1.average() < s2.average()) {
		return (true);
	} else {
		return (false);
	}
}

static int fac(int i) {
	if (i == 0) {
		return (1);
	} else {
		return (i * fac(i - 1));
	}
}

//////////////////////////////////

Distance::Distance(Symbol &cSymbol1, Symbol &cSymbol2, bool bDebug) :
	dTotal(0.0),
	dExMinCost(0.0),
	iExSkippedSteps(0) {

	// Compute the distance from symbol 1 to 2 and store it into score 1
	compute(cScores1, cSymbol1, cSymbol2);

	if (bDebug) {
		printf("Score 1->2:\n");
		cScores1.dump();
		printf("--------------------\n");
	}

	// Compute the distance from symbol 2 to 1 and store it into score 2
	compute(cScores2, cSymbol2, cSymbol1);

	if (bDebug) {
		printf("Score 2->1:\n");
		cScores2.dump();
		printf("====================\n");
	}

	// Clean up old data
	cDrawOrder.clear();
	cDrawOrder.resize(cScores1.size(), -1);

	// Heuristic match
	//heuristicMatch();
	exhaustiveSearch();
	//probabiliticSearch();

	if (bDebug) {
		printf("Drawing order:\n");
		for (int i = 0; i < cDrawOrder.size(); i++) {
			printf("%d: %d\n", i, cDrawOrder[i]);
		}
	}
}

/*
 * Sample given line by one unit apart using unit vector
 */
void Distance::sampleLine(vector<Coord> &cSample, Line &cLine) {
	// Convert end points using vectors
	Vector a(cLine.x1(), cLine.y1());
	Vector b(cLine.x2(), cLine.y2());

	// Make sure vector a < b
	if (a > b) {
		swapVector(a, b);
	}

	// Derive a unit vector from vector a and b
	Vector u = unitVector(a, b) * DISTANCE_SAMPLE_RATE;

	// Sample the line
	Vector v;
	for (v = a; v <= b; v += u) {
		cSample.push_back(Coord(v.x(), v.y()));
	}
	// Make sure last end point be part of sample data
	if (v != b) {
		cSample.push_back(Coord(b.x(), b.y()));
	}
}

/*
 * Sample given arc/curve by one unit apart
 */
void Distance::sampleCurve(vector<Coord> &cSample, Curve &cCurve) {
	// Get the angle of the arc
	double delta = cCurve.endAngle() - cCurve.startAngle();
	assert(delta >= 0);

	// When delta is small, arc length = radius * delta
	// If we want arc length is incremented by 1 unit, the delta angle 
	// should be incremented by 1/r
	double inc = DISTANCE_SAMPLE_RATE * 1.0 / cCurve.radius();

	// Sample the arc
	for (double angle = 0; angle <= delta; angle += inc) {
		double x = cCurve.centerX() +
				   cCurve.radius() * cos(cCurve.startAngle() + angle);
		double y = cCurve.centerY() +
				   cCurve.radius() * sin(cCurve.startAngle() + angle);
		cSample.push_back(Coord(x, y));
	}
}

/*
 * Sample segment 1
 */
void Distance::sampleSegment1(MySegment *s1) {
	// Clear sample 1 first before sampling
	cSample1.clear();

	switch (s1->Type()) {
	case LINE:
		sampleLine(cSample1,
				   Line(s1->StartPoint().x, s1->StartPoint().y,
						s1->EndPoint().x, s1->EndPoint().y));
		break;
	case ARC:
		sampleCurve(cSample1,
					Curve(s1->Center().x, s1->Center().y, s1->Radius(),
						  s1->SmallAngle(), s1->BigAngle()));
		break;
	default:
		// Should not reach here
		fprintf(stderr, "Unknown segment type: %d\n", s1->Type());
		break;
	}
}

/*
 * Sample segment 2 after transformation
 */
void Distance::sampleSegment2(MySegment *s2, Matrix &m,
							  double xScale, double yScale) {
	// Clear sample 2 first before sampling
	cSample2.clear();

	Matrix p1(3, 1), p2(3, 1), pp1(3, 1), pp2(3, 1);
	double r = 0;

	switch (s2->Type()) {
	case LINE:
		// Store original end points of the line into p1 and p2
		p1.data(3, s2->StartPoint().x, s2->StartPoint().y, 1.0);
		p2.data(3, s2->EndPoint().x, s2->EndPoint().y, 1.0);
		// pp1 & pp2 are the new end points after transformation
		pp1 = m * p1;
		pp2 = m * p2;
		// Sample the line
		sampleLine(cSample2,
				   Line(pp1.x(), pp1.y(), pp2.x(), pp2.y()));
		break;
	case ARC:
		// Store center of the arc into p1
		p1.data(3, s2->Center().x, s2->Center().y, 1.0);
		// pp1 is the new center after transformation
		pp1 = m * p1;
		// r is the new radius after transformation
		r = s2->Radius() * min(xScale, yScale);
		// Sample the arc
		sampleCurve(cSample2,
					Curve(pp1.x(), pp1.y(), r,
						  s2->SmallAngle(), s2->BigAngle()));
		break;
	default:
		// Should not reach here
		fprintf(stderr, "Unknown segment type: %d\n", s2->Type());
		break;
	}
}

/*
 * Compute the total distance from one segment to nearest point of another
 */
void Distance::compute(void) {
	dTotal = 0;
	for (int i = 0; i < cSample1.size(); i++) {
		double dMinDist = 1000000;
		for (int j = 0; j < cSample2.size(); j++) {
			double d = cSample1[i].distanceTo(cSample2[j]);
			if (d < dMinDist) {
				dMinDist = d;
			}
		}
		dTotal += dMinDist;
	}
}

void Distance::compute(Scores &cScores,
					   Symbol &cSymbol1, Symbol &cSymbol2) {
	double xScale = (float)(cSymbol1.maxX() - cSymbol1.minX()) /
					(cSymbol2.maxX() - cSymbol2.minX());
	double yScale = (float)(cSymbol1.maxY() - cSymbol1.minY()) /
					(cSymbol2.maxY() - cSymbol2.minY());

	// Translate 2nd symbol back to origin, scale it to 1st symbol's size and
	// translate it back to 1st symbol's position
	Matrix m = MatrixTranslation(cSymbol1.minX(), cSymbol1.minY()) *
			   MatrixScaling(xScale, yScale) *
			   MatrixTranslation(-cSymbol2.minX(), -cSymbol2.minY());

	cScores.clear();
	cScores.resize(cSymbol1.size());
	int i;
	for (i = 0; i < cSymbol1.size(); i++) {
		sampleSegment1(cSymbol1[i]);

		cScores[i].resize(cSymbol2.size());
		for (int j = 0; j < cSymbol2.size(); j++) {
			sampleSegment2(cSymbol2[j], m, xScale, yScale);
			// Compute distance from sample 1 & 2
			compute();
			// Keep track of the average distance
			cScores[i][j] = Score(j, average());
		}
	}

	// Compute the total, average and find the mininum
	for (i = 0; i < cScores.size(); i++) {
		double dTotal = 0.0, dMin = 10000.0;
		int iMinPos = cScores[i].size();
		// Compute total and find the mininum position
		int j;
		for (j = 0; j < cScores[i].size(); j++) {
			dTotal += cScores[i][j].average();
			if (cScores[i][j].average() < dMin) {
				dMin = cScores[i][j].average();
				iMinPos = j;
			}
		}
		// Update the percentage for each segment
		for (j = 0; j < cScores[i].size(); j++) {
			cScores[i][j].percentage(cScores[i][j].average() * 100 / dTotal);
		}
		// Flag the best segment
		cScores[i][iMinPos].best(true);
	}
}

void Distance::heuristicMatch(void) {
	// Try to match segments up based on the scores 1 & 2
	vector<bool> cMark(cScores1.size(), false);

	// Sort scores based on the average score in accending order.
	// ie. cScores1[i][0] would always have the lowest average score
	cScores1.sort();
	cScores2.sort();

	// 1st pass: both distances from s1 to s2 and s2 to s1 are minimum
	int i;
	for (i = 0; i < cDrawOrder.size(); i++) {
		int iMatch1 = cScores1[i][0].match();
		int iMatch2 = cScores2[iMatch1][0].match();
		if (!cMark[iMatch1] && i == iMatch2) {
			cDrawOrder[i] = iMatch1;
			cMark[iMatch1] = true;
		}
	}

	// 2nd pass: do the best matching possible
	for (i = 0; i < cDrawOrder.size(); i++) {
		if (cDrawOrder[i] != -1) {
			continue;
		}

		// Go thru 2nd list to find a good one
		int iMatch1 = cScores1[i][0].match();
		int j = 0;
		for (j = 0; j < cDrawOrder.size(); j++) {
			int iMatch2 = cScores2[iMatch1][j].match();
			if (!cMark[iMatch1] && i == iMatch2) {
				cDrawOrder[i] = iMatch1;
				cMark[iMatch1] = true;
				break;
			}
		}

		// If all taken, go thru the 1st list
		if (j >= cDrawOrder.size()) {
			for (int k = 0; k < cDrawOrder.size(); k++) {
				int iMatch = cScores1[i][k].match();
				if (!cMark[iMatch]) {
					cDrawOrder[i] = iMatch;
					cMark[iMatch] = true;
					break;
				}
			}
		}
	}
}

/*
 * Exhaustive search helper
 */
void Distance::exhaustiveSearch(list<int> &l1, list<int> &l2,
								double dTotalCost) {
	int iSize1 = l1.size();

	if (iSize1 == 0) {
		if (dTotalCost < dExMinCost) {
			dExMinCost = dTotalCost;
			cDrawOrder = cExDrawOrder;
			/*
			printf("Cost = %f\n", dTotalCost);
			for (int i = 0; i < cDrawOrder.size(); i++) {
				printf("%d-%d: %.2f\n",
					   i, cDrawOrder[i],
					   cScores1[i][cDrawOrder[i]].average());
			}
			*/
		}
	} else {
		/*
		// tom hack dec 21 2006 start 
		double heuristic = 0.0;
		for (list<int>::iterator itr = l1.begin(); itr != l1.end(); itr++) {
			int ss = *itr;
			if (ss == -1) {
				continue;
			}
			for (list<int>::iterator itr2 = l2.begin(); itr2 != l2.end(); itr2++) {
				int jj = *itr2;
				if (jj != -1 && cScores1[ss][jj].best()) {
					heuristic += cScores1[ss][jj].average();
					break;
				}
			}
		}
		if (dTotalCost + heuristic >= dExMinCost) {
		// tom hack dec 21 2006 end
		*/
		if (dTotalCost >= dExMinCost) {
			iExSkippedSteps += fac(iSize1);
			//printf("Skip steps = %d\n", iExSkippedSteps);
			return;
		}
		int s1 = l1.front();
		l1.pop_front();
		for (int i = 0; i < l2.size(); i++) {
			double dCost = 0;
			int s2 = l2.front();

			if (s2 != -1) {
				dCost = cScores1[s1][s2].average();
			}
			cExDrawOrder[s1] = s2;
			dTotalCost += dCost;
			l2.pop_front();
			exhaustiveSearch(l1, l2, dTotalCost);
			l2.push_back(s2);
			dTotalCost -= dCost;
		}
		l1.push_back(s1);
	}
}

/*
 * Exhaustive search for finding the corresponding segment pairs with minimal
 * total distance between 2 symbols
 */
void Distance::exhaustiveSearch(void) {
	// Setup 2 lists
	list<int> l1, l2;
	int i;
	for (i = 0; i < cScores1.size(); i++) {
		l1.push_back(i);
	}
	// XXX: should l1 and l2 the same size?
	for (i = 0; i < cScores2.size(); i++) {
		l2.push_back(i);
	}
	// If symbol 2 has lesser number of segments than symbol 1, append
	// dummy segment (-1) for symbol 2
	int iDiff = l1.size() - l2.size();
	for (i = 0; i < iDiff; i++) {
		l2.push_back(-1);
	}

	//printf("Size = %d, Steps = %d\n", l1.size(), fac(l1.size()));

	// Clean up old data
	dExMinCost = 1e10;
	iExSkippedSteps = 0;
	cExDrawOrder.clear();
	cExDrawOrder.resize(cScores1.size(), -1);

	// Start recursive and exhaustive search
	exhaustiveSearch(l1, l2, 0.0);

	//printf("Skipped steps = %d\n", iExSkippedSteps);
}

void Distance::probabiliticSearch(void) {
	// First, blindly assign order
	cDrawOrder.resize(cScores1.size());
	int i;
	for (i = 0; i < cScores1.size(); i++) {
		cDrawOrder[i] = i;
	}

	// Then, start swapping the segments with largest distance
	for (i = 0; i < DISTANCE_PROB_N; i++) {
		vector<float> cError;
		for (int j = 0; j < cScores1.size(); j++) {
			cError.push_back(cScores1[j][cDrawOrder[j]].average());
		}

		DistProb cDistProb(cError);
		int s1 = cDistProb.generate();
		int s2 = s1;

		int iFailSafe = 0;
		while (s2 == s1) {
			if (iFailSafe++ >= DISTANCE_SWAP_FAIL_SAFE) {
				printf("Warning: take too long to converge!\n");
				break;
			}
			s2 = cDistProb.generate();
		}

		// Swap 2 segments
		//printf("Swapping segs %d and %d\n", s1, s2);
		if (s1 != s2) {
			int tmp = cDrawOrder[s1];
			cDrawOrder[s1] = cDrawOrder[s2];
			cDrawOrder[s2] = tmp;
		}
	}
}

void Distance::dump(void) {
	printf("Sample 1:\n");
	int i;
	for (i = 0; i < cSample1.size(); i++) {
		printf("  (%f, %f)\n", cSample1[i].x, cSample1[i].y);
	}
	printf("Sample 2:\n");
	for (i = 0; i < cSample2.size(); i++) {
		printf("  (%f, %f)\n", cSample2[i].x, cSample2[i].y);
	}
}
