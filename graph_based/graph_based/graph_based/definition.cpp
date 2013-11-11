// $Id: definition.cpp,v 1.40 2006/12/27 22:25:25 weesan Exp $

/***********************************************

	Copyright (C) 2002 - 2004
	UC Riverside Smart Tools Lab and Thomas Stahovich 
		

***********************************************/


#include "stdafx.h"
#include "definition.h"
#include "population.h"
#include "timer.h"
#include "debug.h"

#include <math.h>
#include <assert.h>
#include <fstream>
// Added by weesan@cs.ucr.edu
#include <algorithm>

#define DEF_SKIP_LINK

#define DEF_TYPE_MISMATCH_THRESHOLD  (0.15)

using namespace std;

//fix: todo
//delete news in glue.cpp
//test the standard deviation function
//todo: think about changes in drawing direction.
//write defns to file and be able to update them
//observe error convergence from excel plot.


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Global variable
Definitions cDefs;

//////////////////////////////////

#ifdef DEF_LOC_WEIGHT
// Only first 16 bits are used currently
static unsigned int puiBitMask[] = {
	0x0001,
	0x0002,
	0x0004,
	0x0008,
	0x0010,
	0x0020,
	0x0040,
	0x0080,
	0x0100,
	0x0200,
	0x0400,
	0x0800,
	0x1000,
	0x2000,
	0x4000,
	0x8000
};

static int bitmapCount(const unsigned int uiLocBitmap) {
	if (uiLocBitmap == 0) {
		return (0);
	} else {
		int iCount = 0;
		for (int i = 0; i < sizeof(puiBitMask) / sizeof(unsigned int); i++) {
			if (uiLocBitmap & puiBitMask[i]) {
				iCount++;
			}
		}
		return (iCount);
	}
}
#endif

// Sort the training samples in a definition based on the assigned weight
static bool symbolCmp(const Symbol *s1, const Symbol *s2) {
	return (s1->bestDrawOrderWeight() < s2->bestDrawOrderWeight());
}

//////////////////////////////////

Definition::Definition(const char *pzcName) :
	Object("Definition"),
	cName(pzcName ? pzcName : "undefined"),
	numof_segments(0) {
}

// See ValidSymbol() for return value
int Definition::AddTrainSymbol(Symbol* S)
{
	assert(S!=NULL);

	int iRet = ValidSymbol(S);
	// Add only valid symbols
	if (iRet == 0) {
		// Re-arrange the segments' orientation based on their coordinates
		// NOTE: We assume users provide training sample in the same
		// orientation
		S->flipSegments();
		// Have to reprocess the symbol after that
		S->Process();
		// Add it to the definition
		push_back(S);
	}
	return (iRet);
}

/*
 * Modified by weesan@cs.ucr.edu
 * Return code as below:
 *		0: success
 *		1: Fail primitive count check
 *		2: Fail primitive type check
 */
int Definition::ValidSymbol(const Symbol* S)
{
	cMeanNoofSegments.insert(S->size());

	if(size()==0)	
	{
		numof_segments=S->size();
		return (0);
	}

//	numof_segments = max(S->size(), numof_segments);
	numof_segments = (int)cMeanNoofSegments.mean();
	return (0);

#ifdef weesan
	if(S->size()!=numof_segments) 
	{
		//cout<<"Primitive count mismatch...Symbol ignored in training.\n";
		return (1);
	}

	float stTotal = 0, dtTotal = 0;
	for(int i=0;i<numof_segments;i++)
	{
		// Added by weesan@cs.ucr.edu
		MySegment *ss = S->getSegment(i);
		MySegment *ds = (*this)[0]->getSegment(i);

		float st = ss->fuzzyType();
		float dt = ds->fuzzyType();

		/*
		if ((st == 0 || st == 1) && (dt == 0 || dt == 1) && (st != dt)) {
			//cout<<"Primitive type mismatch...Symbol ignored in training.\n";
			return (2);
		}
		*/

		stTotal += st;
		dtTotal += dt;
	}

	float stAve = stTotal / numof_segments;
	float dtAve = dtTotal / numof_segments;

	//printf("| %f - %f | = %f\n", stAve, dtAve, fabs(stAve - dtAve));

	if (fabs(stAve - dtAve) <= DEF_TYPE_MISMATCH_THRESHOLD) {
		return (0);
	} else {
		return (2);
	}
#endif
}

void Definition::process(int iFirstN)
{
	int i,j,s;

	int iSize = iFirstN ? min(iFirstN, size()) : size();
	if (iSize == 0) {
		return;
	}
	
	// Reset definition's relation table
	RelTable.clear_table();

	/*
	 * Compute mean and stddev of relative length of each segment
	 * from each symbol
	 */
	for(i = 0; i < numof_segments; i++) {
		IntrinsicProp *ip = NULL;
		Population RelLenPop;
		Population cFuzzyType;
		unsigned int uiLocBitmap = 0;
		/* 
		 * For each segment from different symbols,
		 * compute mean and stddev of its rel. length
		 */
		for(s = 0; s < iSize; s++) {
			// Compute the mean and dev. of relative length of ith segment
			// from all training samples
			if ((ip = (*this)[s]->getIP(i)) == NULL) {
				continue;
			}
			RelLenPop.insert(ip->RelLen());
			// Compute the mean and dev. of fuzzy type of ith segment
			cFuzzyType.insert(ip->fuzzyType());
			// Compute the location bitmap of ith segment from all training
			// samples
			uiLocBitmap |= ip->locBitmap();
		}
		/*
		 * XXX: Getting segment type from last symbol,
		 * could be problematic if we are trying to improve train procedure
		 * by allowing symbols with different segment number and types
		 */
		RelTable.set(IntrinsicProp(cFuzzyType.mean() <= 0.5 ? LINE : ARC,
								   cFuzzyType.mean(),
								   cFuzzyType.stdev(),
								   RelLenPop.mean(),
								   RelLenPop.stdev(),
								   uiLocBitmap),
					 i);
	}

	for(i=0;i<numof_segments;i++) 
	{
		for(j=i+1;j<numof_segments;j++) 
		{
			Relationship R, *r = NULL;
			Population IntAnglePop,OneIntPop,TwoIntPop;
			Population IntLocA1Pop,IntLocA2Pop,IntLocB1Pop,IntLocB2Pop;
			bool bSetType = false;

			for(s=0;s<iSize;s++)
			{
				bool bSwap = false;

				if ((r = (*this)[s]->getRelation(i,j, &bSwap)) == NULL) {
					continue;
				}

				if (!bSetType) {
					bSetType = true;
					R.setaType(r->AType());
					R.setbType(r->BType());
				}

				if(r->NumofIntersections()==1) 
				{
					OneIntPop.insert(1.0);
					if (bSwap) {
						IntLocA1Pop.insert(r->IntLocB1());
						IntLocB1Pop.insert(r->IntLocA1());
					} else {
						IntLocA1Pop.insert(r->IntLocA1());
						IntLocB1Pop.insert(r->IntLocB1());
					}
				}
				else
				{
					OneIntPop.insert(0.0);
				}

				if(r->NumofIntersections()==2)
				{
					TwoIntPop.insert(1.0);
					if (bSwap) {
						IntLocA1Pop.insert(r->IntLocA1());
						IntLocA2Pop.insert(r->IntLocA2());
						IntLocB1Pop.insert(r->IntLocB1());
						IntLocB2Pop.insert(r->IntLocB2());
					} else {
						IntLocA1Pop.insert(r->IntLocA1());
						IntLocA2Pop.insert(r->IntLocA2());
						IntLocB1Pop.insert(r->IntLocB1());
						IntLocB2Pop.insert(r->IntLocB2());
					}
				}
				else		
				{
					TwoIntPop.insert(0.0);
				}

				if(r->BothAreLines())
				{
					IntAnglePop.insert(r->IntAngle());
				}
			}

			// XXX: r gets set inside the for loop above and will be undefined
			// if size() == 0.  Should think of a better to deal with it
			//R.setaType(r->AType());
			//R.setbType(r->BType());

			
			if(OneIntPop.mean() > INTERSECTION_FREQ_THRESHOLD)
			{
			  R.setNumofIntersections(1);
			  R.setIntLocA1(IntLocA1Pop.mean(),IntLocA1Pop.stdev());
			  R.setIntLocB1(IntLocB1Pop.mean(),IntLocB1Pop.stdev());
		
			}
			if(TwoIntPop.mean() > INTERSECTION_FREQ_THRESHOLD)
			{
			  R.setNumofIntersections(2);
			  R.setIntLocA1(IntLocA1Pop.mean(),IntLocA1Pop.stdev());
			  R.setIntLocA2(IntLocA2Pop.mean(),IntLocA2Pop.stdev());
			  R.setIntLocB1(IntLocB1Pop.mean(),IntLocB1Pop.stdev());
			  R.setIntLocB2(IntLocB2Pop.mean(),IntLocB2Pop.stdev());	
			}

			if(OneIntPop.mean() < INTERSECTION_FREQ_THRESHOLD && 
			   TwoIntPop.mean() < INTERSECTION_FREQ_THRESHOLD &&
			   OneIntPop.mean()+TwoIntPop.mean() > INTERSECTION_FREQ_THRESHOLD )
			{
				R.setNumofIntersections(1);
				R.setIntLocA1(IntLocA1Pop.mean(),IntLocA1Pop.stdev());
				R.setIntLocB1(IntLocB1Pop.mean(),IntLocB1Pop.stdev());
			}

			// XXX
			//if(r->BothAreLines())
			{	
				R.setIntAngle(IntAnglePop.mean(),IntAnglePop.stdev());
			}

			RelTable.set(R, i, j);
		}
	}

	// Added by weesan@cs.ucr.edu
#ifdef DEF_CENTROID_WEIGHT
	for (i = 0; i < iSize; i++) {
		cCentroid.x += (*this)[i]->centroid().x;
		cCentroid.y += (*this)[i]->centroid().y;
	}
	cCentroid.x /= iSize;
	cCentroid.y /= iSize;
#endif
}

double Definition::PercentMatch(const Symbol* S,
								bool bTrainingMode,
								double dDeviation,
								double dPrimTypeWeight,
								double dPrimCountWeight,
								double dRelLenWeight,
								double dIntNoWeight,
								double dIntAngWeight,
								double dIntLocWeight
#ifdef DEF_CONN_WEIGHT
								, double dConnWeight
#endif
#ifdef DEF_CENTROID_WEIGHT
								, double dCentroidWeight					
#endif
#ifdef DEF_LOC_WEIGHT
								, double dLocWeight
#endif
								) const
{
	int i,j,MINSEG,R;
	Relationship *dr = NULL, *sr = NULL;
	IntrinsicProp *dip = NULL,*sip = NULL;
	double PrimTypeError = 0.0, PrimCountError = 0.0;
	double RelLenError = 0.0, NumofIntersectionError = 0.0;
	double IntersectionAngleError = 0.0, IntLocError = 0.0;

	//MINSEG = min(numof_segments,S->size());
	MINSEG = NumofSegments();
	if(MINSEG==0) return 0.0;

	R=0; //number of relationships
	for(i=0;i<MINSEG;i++) {for(j=i+1;j<MINSEG;j++){R++;}}

	//1) Prim Type Error
/*
	for (i = 0; i < MINSEG; i++) {
		if ((sip = S->getIP(i)) == NULL) {
			continue;
		}
		dip=RelTable.get(i);
		double dProb = normalized_gauss_prob(
			dip->fuzzyType(),
			dDeviation == 0.0 ? dip->fuzzyTypeStdev() : dip->fuzzyType() * dDeviation,
			sip->fuzzyType());
		PrimTypeError += 1.0 - dProb;
	}
	PrimTypeError/=(double)MINSEG;
*/
	for(i=0;i<MINSEG;i++) {
		if ((sip = S->getIP(i)) == NULL) {
			//PrimTypeError += 1.0;
			continue;
		}
		if (RelTable.get(i)->SType() != sip->SType()) {
			PrimTypeError += 1.0;
		}
	}
	PrimTypeError/=(double)MINSEG;

	//2) Prim Count Error
	PrimCountError=(double)fabs((double)numof_segments-(double)S->size())/(double)numof_segments;
	//PrimCountError=(double)fabs(cMeanNoofSegments.mean()-(double)S->size())/(double)numof_segments;
	PrimCountError=min(1.0,PrimCountError);
	
	//3) Relative Length Error
	for(i=0;i<MINSEG;i++)
	{
		if ((sip = S->getIP(i)) == NULL) {
			//RelLenError += 1.0;
			continue;
		}
		dip=RelTable.get(i);
		RelLenError += 1.0 - normalized_gauss_prob(
			dip->RelLen(),
			dDeviation == 0.0 ? dip->RelLen_stdev() : dip->RelLen() * dDeviation,
			sip->RelLen());
	}
	RelLenError/=(double)MINSEG;

	//4) Number of Intersection Error
	int di = 0, si = 0;
	for(i=0;i<MINSEG;i++) {	for(j=i+1;j<MINSEG;j++)
	{
		if ((sr = S->getRelation(i,j)) == NULL) {
			continue;
		}
		dr=RelTable.get(i,j);
		di = dr->NumofIntersections();
		si = sr->NumofIntersections();
		NumofIntersectionError += abs(di - si);
	}}
	if(R!=0) NumofIntersectionError/=(double)R;
	else     NumofIntersectionError=0;
	NumofIntersectionError = 1.0/(1.0+exp(6.0*(1-NumofIntersectionError)));
	//this is a sigmoid function that amplifies large NumofIntersectionErrors and attenuates small NumofIntersectionErrors
	//see gaussian.mcd

	//5) Intersection Angle Error
	int BothLinesCount=0;
	for(i=0;i<MINSEG;i++) {	for(j=i+1;j<MINSEG;j++)
	{
		if ((sr = S->getRelation(i,j)) == NULL) {
			continue;
		}
		dr=RelTable.get(i,j);

		// Ignore virtual intersection angle, added by weesan@cs.ucr.edu
		// XXX: it's doesn't affect the result.  ie. this is not the culprit
		/*
		if (bTrainingMode &&
			(!dr->NumofIntersections() || !sr->NumofIntersections())) {
			printf("Ignoring segments %d and %d\n", i, j);
			continue;
		}
		*/

		if(dr->BothAreLines() && sr->BothAreLines())
		{
			BothLinesCount++;
			IntersectionAngleError += 1.0 - normalized_gauss_prob(
				dr->IntAngle(),
				dDeviation == 0.0 ? dr->IntAngle_stdev() : dr->IntAngle() * dDeviation,
				sr->IntAngle());
		}
	}}
	if(BothLinesCount==0) IntersectionAngleError=0.0; //fix
	else                  IntersectionAngleError/=(double)BothLinesCount;
	
	//6) Intersection Location Error
	int IntlocationCount=0;
	for(i=0;i<MINSEG;i++) {	for(j=i+1;j<MINSEG;j++)
	{
		if ((sr = S->getRelation(i,j)) == NULL) {
			continue;
		}
		dr=RelTable.get(i,j);

		if(dr->NumofIntersections() && sr->NumofIntersections())
		{
			if(dr->NumofIntersections()==1)
			{
				IntlocationCount+=2; //add 2 because you are considering both A1 and B1, hence for normalization count 2 terms
				IntLocError += 1.0 - normalized_gauss_prob(
					dr->IntLocA1(),
					dDeviation == 0.0 ? dr->IntLocA1_stdev() : dr->IntLocA1() * dDeviation,
					sr->IntLocA1());
				IntLocError += 1.0 - normalized_gauss_prob(
					dr->IntLocB1(),
					dDeviation == 0.0 ? dr->IntLocB1_stdev() : dr->IntLocB1() * dDeviation,
					sr->IntLocB1());
			}
			if(dr->NumofIntersections()==2)
			{
				IntlocationCount+=4; //add 4 because you are considering both A1,B1,A2,B2, hence for normalization count 4 terms
				IntLocError += 1.0 - normalized_gauss_prob(
					dr->IntLocA1(),
					dDeviation == 0.0 ? dr->IntLocA1_stdev() : dr->IntLocA1() * dDeviation,
					sr->IntLocA1());
				IntLocError += 1.0 - normalized_gauss_prob(
					dr->IntLocB1(),
					dDeviation == 0.0 ? dr->IntLocB1_stdev() : dr->IntLocB1() * dDeviation,
					sr->IntLocB1());
				IntLocError += 1.0 - normalized_gauss_prob(
					dr->IntLocA2(),
					dDeviation == 0.0 ? dr->IntLocA2_stdev() : dr->IntLocA2() * dDeviation,
					sr->IntLocA2());
				IntLocError += 1.0 - normalized_gauss_prob(
					dr->IntLocB2(),
					dDeviation == 0.0 ? dr->IntLocB2_stdev() : dr->IntLocB2() * dDeviation,
					sr->IntLocB2());
			}
		} else {
//		} else if (dr->NumofIntersections() || sr->NumofIntersections()) {
			int iNoofInt = max(1, max(dr->NumofIntersections(),
							          sr->NumofIntersections()));
			int iPenalty = iNoofInt * 2;
			IntlocationCount += iPenalty;
			IntLocError += iPenalty;
		}
	}}
	if(IntlocationCount==0) IntLocError=0.0;
	else                  IntLocError/=(double)IntlocationCount;

#ifdef DEF_CONN_WEIGHT
	// Added by weesan@cs.ucr.edu
	// 7) Connectivity for each segment with other segments
	double dConnError = 0;
	for (i = 0; i < MINSEG; i++) {
		int dConn = 0, sConn = 0;
		// NOTE: j = 0 not j = i + 1 as previous ones because we need the
		// intersection number from one segment to any others
		for (j = 0; j < MINSEG; j++) {
			if (i == j) {
				continue;
			} else {
				int a = i, b = j;
				if (a > b) {
					int t = a;
					a = b;
					b = t;
				}
				dr = RelTable.get(a, b);
				sr = S->getRelation(a, b);
				dConn += dr->NumofIntersections();
				sConn += sr->NumofIntersections();
			}
		}
		dConnError += abs(dConn - sConn);
	}
	// Average out by the number of segments
	dConnError /= MINSEG;
	// Potentially, 1 segment might intersect 2 pts with other segments,
	// which make dConnError ranged from [0, 2].
	// A squash function is applied to avoid that.
	dConnError = 1.0 / (1.0 + exp(6.0 * (1 - dConnError)));
#endif

	// Added by weesan@cs.ucr.edu
	// 8) Centroid of the symbol
#ifdef DEF_CENTROID_WEIGHT
	double eDistance = centroid().distanceTo(S->centroid());
	double dCentroidError = min(50.0, eDistance) / 50.0;
#endif

	// Added by weesan@cs.ucr.edu
	// 9) Segment location
#ifdef DEF_LOC_WEIGHT
	double dLocError = 0;
	for(i = 0; i < MINSEG; i++) {
		dip=RelTable.get(i);
		sip=S->getIP(i);
		int iDefBitmap = dip->locBitmap();
		int iSymBitmap = sip->locBitmap();
		if (iDefBitmap != 0) {
			dLocError += 1 - ((float)bitmapCount(iDefBitmap & iSymBitmap) / 
						      bitmapCount(iDefBitmap));
		}
	}
	dLocError /= MINSEG;
#endif

	assert(PrimTypeError>=0.0 && PrimTypeError<=1.0);
	assert(PrimCountError>=0.0 && PrimCountError<=1.0);
	// XXX: Crashed in the case of full circle
	assert(RelLenError>=0.0 && RelLenError<=1.0);
	assert(NumofIntersectionError>=0.0 && NumofIntersectionError<=1.0);
	assert(IntersectionAngleError>=0.0 && IntersectionAngleError<=1.0);
	assert(IntLocError>=0.0 && IntLocError<=1.0);
#ifdef DEF_CONN_WEIGHT
	assert(dConnError >= 0.0 && dConnError <= 1.0);
#endif
#ifdef DEF_CENTROID_WEIGHT
	assert(dCentroidError >= 0.0 && dCentroidError <= 1.0);
#endif
#ifdef DEF_LOC_WEIGHT
	assert(dLocError >= 0.0 && dLocError <= 1.0);
#endif

	// Reset errors in symbol
	((Symbol *)S)->clearErrors();
	// Save all the errors
	((Symbol *)S)->saveError(PrimTypeError);
	((Symbol *)S)->saveError(PrimCountError);
	((Symbol *)S)->saveError(RelLenError);
	((Symbol *)S)->saveError(NumofIntersectionError);
	((Symbol *)S)->saveError(IntersectionAngleError);
	((Symbol *)S)->saveError(IntLocError);
#ifdef DEF_CONN_WEIGHT
	((Symbol *)S)->saveError(dConnError);
#endif
#ifdef DEF_CENTROID_WEIGHT
	((Symbol *)S)->saveError(dCentroidError);
#endif
#ifdef DEF_LOC_WEIGHT
	((Symbol *)S)->saveError(dLocError);
#endif

	return 1-(PrimTypeError * dPrimTypeWeight +
			  PrimCountError * dPrimCountWeight +
		      RelLenError * dRelLenWeight +
			  NumofIntersectionError * dIntNoWeight +
			  IntersectionAngleError * dIntAngWeight +
			  IntLocError * dIntLocWeight
#ifdef DEF_CONN_WEIGHT
			  + dConnError * dConnWeight
#endif
#ifdef DEF_CENTROID_WEIGHT
			  + dCentroidError * dCentroidWeight
#endif
#ifdef DEF_LOC_WEIGHT
			  + dLocError * dLocError
#endif
			  );
}

Relationship* Definition::getRelation(int indexA,int indexB) const
{
	assert(indexA<NumofSegments() && indexB<NumofSegments() &&  indexA>=0 && indexB>=0);
	assert(indexA != indexB);

	if (indexA > indexB) {
		return (RelTable.get(indexB,indexA));
	} else {
		return (RelTable.get(indexA,indexB));
	}
}

IntrinsicProp* Definition::getIP(int indexA) const
{
	assert(indexA<NumofSegments() && indexA>=0);
	return RelTable.get(indexA);
}

int Definition::NumofSegments() const
{
	return numof_segments;
}

void Definition::print() const
{
	cout<<endl;
	RelTable.print();
}

void Definition::write(std::ostream &os) const {
	for (int i = 0; i < size(); i++) {
		os << *(*this)[i];
	}
}

// Sort the order of symbols (training samples) in the definition 
// in ascending order based on the assigned weight
void Definition::sort(void) {
	// Sort the order of symbols (training samples) in the definition
	std::sort(begin(), end(), symbolCmp);
}

//////////////////////////////////

char Definitions::resultCode(int iResult) {
	switch (iResult) {
	case 0:
		return ('.');
		break;
	case 1:
		return ('-');
		break;
	case 2:
		return ('!');
		break;
	default:
		return ('+');
		break;
	}
}

bool Definitions::write(const char *pzcOutDir) {
	for (int i = 0; i < size(); i++) {
		char pzcFile[_MAX_PATH + 1];
		snprintf(pzcFile, sizeof(pzcFile),
				 "%s\\%s.txt", pzcOutDir, (*this)[i]->name());
		fstream ofs(pzcFile, ios::out);
		if (!ofs) {
			fprintf(stderr, "Failed to open %s", pzcFile);
			return (false);
		} else {
			ofs << *(*this)[i];
		}
	}

	return (true);
}

/*
 * Read symbols one by one and add them into the definition.
 * It assumes all symbols are drawn in the same order.
 */
void Definitions::read(const char *pzcDefDir) {
	char pwd[_MAX_PATH + 1];

	if (pzcDefDir && *pzcDefDir) {
		snprintf(pwd, sizeof(pwd), "%s", pzcDefDir);
	} else {
		int iSize = GetCurrentDirectory(sizeof(pwd), pwd);
		assert(iSize != 0);

		snprintf(pwd, sizeof(pwd), "%s\\%s", pwd, DEF_DIR);
	}

	printf("Reading definition symbols from %s ...\n", pwd);

	Symbols cSymbols(pwd);
	for (bool bRet = cSymbols.firstFile(); bRet; bRet = cSymbols.nextFile()) {
		// Retrieve the name of the definition without the extension
		char pzcName[_MAX_PATH + 1];
		cSymbols.name(pzcName);

		Definition *pcDef = new Definition(pzcName);
		assert(pcDef != NULL);

		printf("  %s ", cSymbols.filename());

		for (Symbol *pcSymbol = cSymbols.firstSymbol();
			 pcSymbol;
			 pcSymbol = cSymbols.nextSymbol()) {
			// Add the symbol into definition
			int iResult = pcDef->AddTrainSymbol(pcSymbol);
			if (iResult != 0) {
				delete pcSymbol;
			}
			printf("%c", resultCode(iResult));
		}

		pcDef->process();
		push_back(pcDef);
		printf(" %d symbols\n", pcDef->size());
	}
	printf("\n");
}

void Definitions::addSymbol(Symbol *pcSymbol, Definition *pcDef,
							Match::Type eMatch, bool bDistance,
							int iVerbose) {
	int iResult = -1;

	// Restore its original order
	pcSymbol->restoreOrigOrder();

	//printf("==> %d\n", pcDef->size());
	if (pcDef->size() == 0) {
		// If it's first symbol, simply add the symbol into definition.
		iResult = pcDef->AddTrainSymbol(pcSymbol);
		if (iVerbose) {
			printf("%c", resultCode(iResult));
		}
		if (iResult == 0) {
			pcDef->process();
		} else {
			delete pcSymbol;
		}
	} else {
		// Otherwise, treat this symbol as a test symbol and find
		// its the best drawing order
		Match *pcMatch = NULL;
		if (bDistance) {
			// Use distance among segments to determine segment order
			//(*pcDef)[0]->match(*pcSymbol);
			pcMatch = Match::allocate(Match::DISTANCE, pcSymbol, true);
		} else {
			// Use existing matching algorithm instead
			pcMatch = Match::allocate(eMatch, pcSymbol, true);
		}
		assert(pcMatch != NULL);
		pcMatch->findBestDrawOrder(pcDef);
		delete pcMatch;

		do {
			// Restore the best order for the symbol
			pcSymbol->restoreBestOrder();
			// Try to add it into the definition
			iResult = pcDef->AddTrainSymbol(pcSymbol);
			if (iVerbose) {
				printf("%c", resultCode(iResult));
			}
			if (iResult == 0) {
				// Ok to add symbol into definition
				pcDef->process();
				break;
			} else {
				// Fail to add symbol into definition, try next best
				if (!pcSymbol->popBestDrawOrder()) {
					delete pcSymbol;
					break;
				}
			}
		} while (1);
	}
}

/*
 * Same as Definitions::read(), but, it does not assume same drawing order
 * for all training samples.
 * Here is how it works: it reads the 1st symbol and use it as a definition
 * to find better drawing order for 2nd symbol.  Once such order is found,
 * the 2nd symbol will be added to the definition.  Then 3rd symbol is read
 * and compared against the first 2 symbols.  This process goes on until
 * all the symbols are read.
 */
void Definitions::readIncrement(const char *pzcDefDir, Match::Type eMatch,
								int iRandom, bool bDistance) {
	Timer cTimer;
	char pwd[_MAX_PATH + 1];

	if (pzcDefDir && *pzcDefDir) {
		snprintf(pwd, sizeof(pwd), "%s", pzcDefDir);
	} else {
		int iSize = GetCurrentDirectory(sizeof(pwd), pwd);
		assert(iSize != 0);

		snprintf(pwd, sizeof(pwd), "%s\\%s", pwd, DEF_DIR);
	}

	printf("Reading definition symbols from %s ", pwd);
	if (iRandom) {
		printf("(random) ");
	}
	printf("...\n", pwd);

	Symbols cSymbols(pwd, iRandom);
	for (bool bRet = cSymbols.firstFile(); bRet; bRet = cSymbols.nextFile()) {
		// Retrieve the name of the definition without the extension
		char pzcName[_MAX_PATH + 1];
		cSymbols.name(pzcName);
#ifdef DEF_SKIP_LINK
		if (strcmp(pzcName, "link") == 0) {
			continue;
		}
#endif

		Definition *pcDef = new Definition(pzcName);
		assert(pcDef != NULL);

		printf("  %s ", cSymbols.filename());
		for (Symbol *pcSymbol = cSymbols.firstSymbol();
			 pcSymbol;
			 pcSymbol = cSymbols.nextSymbol()) {
			cTimer.start();
			addSymbol(pcSymbol, pcDef, eMatch, bDistance);
			cTimer.finish();
		}

		push_back(pcDef);
		printf(" %d symbols\n", pcDef->size());
	}
	printf("Processing time is %.02fms per symbol.\n\n", cTimer.average());
}

/*
 * Re-orders the segments of each symbol in the definition by comparing
 * each symbol from the definition against the definitions.
 */
void Definitions::feedback(Match::Type eMatch) {
	for (int i = 0; i < size(); i++) {
		// Get the definition
		Definition *pcDef = (*this)[i];
		for (int j = 0; j < pcDef->size(); j++) {
			// Get the symbol
			Symbol *pcSymbol = (*pcDef)[j];
			//cout << (*pcSymbol) << endl;
			// Get the match function
			Match *pcMatch = Match::allocate(eMatch, pcSymbol, true);
			assert(pcMatch != NULL);
			// Find the best match for the symbol in the definition
			pcMatch->findBestDrawOrder(pcDef);
			delete pcMatch;
			// Restore the best order for the symbol
			pcSymbol->restoreBestOrder();
		}
		// Re-process the symbols of the definition
		pcDef->process();
	}
}

/*
 * Like Definitions::feedback(), an extension to Definitions::readIncrement(),
 * for each definition, this function will use first half of the training
 * symbols to determine the draw order of the rest of the symbols for the
 * definition.
 */
void Definitions::bestN(Match::Type eMatch, int n) {
	// Call feedback() to compute the best/least error of each symbol
	// in a definition
	feedback(eMatch);

	for (int i = 0; i < size(); i++) {
		// Get the definition
		Definition *pcDef = (*this)[i];
		int k = (n <= 1) ? pcDef->size() / 2 : n;
		if (k <= 1) {
			// It only makes sense when k >= 2
			continue;
		}
		// Sort the symbols based on the total error in ascending order
		// in a definition
		pcDef->sort();
		// Process only first k symbol
		pcDef->process(k);
		// The rest will be processed after finding the best drawing order
		for (int j = k; j < pcDef->size(); j++) {
			// Get the symbol
			Symbol *pcSymbol = (*pcDef)[j];
			// Get the match function
			Match *pcMatch = Match::allocate(eMatch, pcSymbol, true);
			assert(pcMatch != NULL);
			// Find the best match for the symbol in the definition
			pcMatch->findBestDrawOrder(pcDef);
			delete pcMatch;
			// Restore the best order for the symbol
			pcSymbol->restoreBestOrder();
		}
		// Re-process ALL symbols of the definition
		pcDef->process();
	}
}

void Definitions::dumpDrawOrder(void) {
	printf("Drawing order of definitions' symbols:\n");
	for (int i = 0; i < size(); i++) {
		// Get the definition
		Definition *pcDef = (*this)[i];
		printf("%s:\n", pcDef->name());
		// Get the symbol
		for (int j = 0; j < pcDef->size(); j++) {
			Symbol *pcSymbol = (*pcDef)[j];
			printf("  %2d: %s\n", j + 1, pcSymbol->drawOrder());
		}
	}
	printf("\n");
}

void Definitions::dumpIncStdDev(void) {
	printf("Definitions' incremental standard deviation:\n");
	for (int i = 0; i < size(); i++) {
		// Get the definition
		Definition *pcDef = (*this)[i];
		Population *pcPop = new Population[(*pcDef)[0]->size()];
		assert(pcPop != NULL);
		printf("%s:\n", pcDef->name());
		// Get the symbol
		for (int j = 0; j < pcDef->size(); j++) {
			Symbol *pcSymbol = (*pcDef)[j];
			// Get the segment
			int k;
			for (k = 0; k < pcSymbol->size(); k++) {
				MySegment *pcSegment = (*pcSymbol)[k];
				pcPop[k].insert(pcSegment->RelLength(pcSymbol->totalLength()));
			}
			printf("%d: ", j);
			for (k = 0; k < pcSymbol->size(); k++) {
				printf("%.02f ", pcPop[k].stdev());
			}
			printf("\n");
		}
		delete []pcPop;
	}
	printf("\n");
}

void Definitions::dumpGraph(void) {
	printf("Definitions' relation graph:\n");
	for (int i = 0; i < size(); i++) {
		Definition *pcDef = (*this)[i];
		printf("%s:\n", pcDef->name());
		for (int i = 0; i < pcDef->NumofSegments(); i++) {
			for (int j = i; j < pcDef->NumofSegments(); j++) {
				if (i == j) {
					IntrinsicProp *pcProp = pcDef->getIP(i);
					printf("  %d-%c: RL=%.3f(%.3f)\n",
							i,
							pcProp->SType() == LINE ? 'L' : 'A',
						    pcProp->RelLen(), pcProp->RelLen_stdev());
				} else {
					Relationship *pcRel = pcDef->getRelation(i, j);
					printf("  %d-%d: IN=%d, IA=%.1f(%.1f), "
							"IL=%.02f,%.02f(%.02f,%.02f)\n",
							i, j,
							pcRel->NumofIntersections(),
							Rad2Deg(pcRel->IntAngle()),
							Rad2Deg(pcRel->IntAngle_stdev()),
							pcRel->IntLocA1(), pcRel->IntLocB1(),
							pcRel->IntLocA1_stdev(), pcRel->IntLocB1_stdev());
				}
			}
		}
	}
	printf("\n");
}

// Draw definitions graph in dot format
// http://www.graphviz.org/
bool Definitions::plotGraph(const char *pzcOutDir) {
	for (int i = 0; i < size(); i++) {
		char pzcFile[_MAX_PATH + 1];
		snprintf(pzcFile, sizeof(pzcFile),
				 "%s\\%s.dot", pzcOutDir, (*this)[i]->name());
		FILE *fp = fopen(pzcFile, "w");
		if (!fp) {
			fprintf(stderr, "Failed to open %s", pzcFile);
			// Output as many definitions as possible
			continue;
		} else {
			fprintf(fp,
					"graph {\n"
					"\tnode [shape=ellipse]\n"
					"\tedge [len=5]\n");
			Definition *pcDef = (*this)[i];
			for (int i = 0; i < pcDef->NumofSegments(); i++) {
				for (int j = i; j < pcDef->NumofSegments(); j++) {
					if (i == j) {
						IntrinsicProp *pcProp = pcDef->getIP(i);
						fprintf(fp,
								"\t%d [label=\"%s %d\\nRL=%.3f(%.3f)\"];\n",
								i,
								pcProp->SType() == LINE ? "Line" : "Arc",
								i,
							    pcProp->RelLen(), pcProp->RelLen_stdev());
					} else {
						Relationship *pcRel = pcDef->getRelation(i, j);
						fprintf(fp,
								"\t%d -- %d [label=\""
								"IN=%d\\nIA=%.1f (%.1f)\\n"
								"IL=%.02f,%.02f (%.02f,%.02f)"
								"\"];\n",
								i, j,
								pcRel->NumofIntersections(),
								Rad2Deg(pcRel->IntAngle()),
								Rad2Deg(pcRel->IntAngle_stdev()),
								pcRel->IntLocA1(), pcRel->IntLocB1(),
								pcRel->IntLocA1_stdev(), pcRel->IntLocB1_stdev());
					}
				}
			}
			fprintf(fp, "}\n");
			fclose(fp);
		}
	}

	return (true);
}

void Definitions::readTrainingSymbols(TrainingSymbols &cTrainingSymbols,
									  int iVerbose) {
	for (TrainingSymbols::iterator itr = cTrainingSymbols.begin();
		 itr != cTrainingSymbols.end();
		 itr++) {
		Symbols &cSymbols = itr->second;
		Definition *pcDef = new Definition(itr->first.c_str());
		assert(pcDef != NULL);
		if (iVerbose) {
			printf("%10s: ", itr->first.c_str());
		}
		for (int i = 0; i < cSymbols.size(); i++) {
			Symbol *pcSymbol = cSymbols[i];
			//Symbol *pcSymbol = new Symbol(*cSymbols[i]);
			if (pcSymbol->mark()) {
				// Use proxity/distance matching
				addSymbol(pcSymbol, pcDef, Match::NULL_MATCH, true, iVerbose);
			}
		}
		push_back(pcDef);
		if (iVerbose) {
			printf(" %d symbols\n", pcDef->size());
		}
	}
}
