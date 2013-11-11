// $Id: distance.h,v 1.3 2005/12/16 00:00:54 weesan Exp $

/*
 * distance.h - Compute the distance between 2 segments (lines and arcs)
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	10/13/2005
 */

#ifndef DISTANCE_H
#define DISTANCE_H

// Silent some warnings from the compile for using vector of vector :(
#pragma warning(disable:4786)
#include <vector>
#include <list>
#include "line.h"
#include "curve.h"
#include "Coord.h"
#include "symbol.h"
#include "matrix.h"

using namespace std;

class Distance {
public:
	class Score {
	private:
		int iMatch;
		double dAverage;
		double dPercentage;
		bool bBest;

	public:
		Score(int iMatch = 0, double dAverage = 0.0) :
			iMatch(iMatch),
			dAverage(dAverage),
			bBest(false) {
		}
		int match(void) const {
			return (iMatch);
		}
		double average(void) const {
			return (dAverage);
		}
		double percentage(void) const {
			return (dPercentage);
		}
		void percentage(double _dPercentage) {
			dPercentage = _dPercentage;
		}
		bool best(void) const {
			return (bBest);
		}
		void best(bool _bBest) {
			bBest = _bBest;
		}
	};

	class Scores : public vector<vector<Score> > {
	private:
		static bool scoreCmp(const Score &s1, const Score &s2) {
			if (s1.average() < s2.average()) {
				return (true);
			} else {
				return (false);
			}
		}

	public:
		void sort(void) {
			for (int i = 0; i < size(); i++) {
				std::sort((*this)[i].begin(), (*this)[i].end(), scoreCmp);
			}
		}
		void dump(void) {
			for (int i = 0; i < size(); i++) {
				for (int j = 0; j < (*this)[i].size(); j++) {
					printf("%c %d - %d: %7.02f (%.02f)\n",
						   (*this)[i][j].best() ? '*' : ' ',
						   i,
						   (*this)[i][j].match(),
						   (*this)[i][j].average(),
						   (*this)[i][j].percentage());
				}
			}
		}
	};

private:
	double dTotal;
	vector<Coord> cSample1, cSample2;
	Scores cScores1, cScores2;
	vector<int> cDrawOrder;
	double dExMinCost;
	int iExSkippedSteps;
	vector<int>cExDrawOrder;

private:
	void sampleLine(vector<Coord> &cSample, Line &cLine);
	void sampleCurve(vector<Coord> &cSample, Curve &cCurve);
	void sampleSegment1(MySegment *s1);
	void sampleSegment2(MySegment *s2, Matrix &m,
						double xScale, double yScale);
	void compute(void);
	void compute(Scores &cScores, Symbol &cSymbol1, Symbol &cSymbol2);
	void heuristicMatch(void);
	void exhaustiveSearch(list<int> &l1, list<int> &l2, double dTotalCost);
	void exhaustiveSearch(void);
	void probabiliticSearch(void);

public:
	Distance(Symbol &cSymbol1, Symbol &cSymbol2, bool bDebug = false);
	double total(void) const {
		return (dTotal);
	}
	double average(void) const {
		return (total() / cSample1.size());
	}
	const vector<int> &drawOrder(void) const {
		return (cDrawOrder);
	}
	void dump(void);
};

#endif // DISTANCE