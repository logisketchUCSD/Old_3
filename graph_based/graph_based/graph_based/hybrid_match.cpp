// $Id: hybrid_match.cpp,v 1.2 2007/04/18 00:33:10 weesan Exp $

/*
 * hybrid_match.cpp - Hybrid matching
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	12/27/2006
 */

#include "hybrid_match.h"
#include "permutation.h"
#include "definition.h"

#ifdef NOT_USED
#include <iostream>
#include <list>
#include <iterator>
using namespace std;






class snode {
public:
  	int dat;
	snode();
	snode(int c);
	snode(const snode& sn);
	~snode();
	void operator=(const snode& sn);
	void print(void);
	bool operator<(snode sn);
	int cost(void);
};

void lprint(list<snode> ls);


snode::snode()
{
	dat = -1;
}

snode::snode(int c)
{
	dat = c;
//	cout << "constructor with " << c << endl;
}

snode::snode(const snode& sn)
{
	dat = sn.dat;
//	cout << "copy constructor " <<  sn.dat << endl;
}

void snode::operator=(const snode& sn)
{
	dat = sn.dat;
//	cout << "assignment operator " << sn.dat << endl;
}

bool snode::operator<(snode sn)
{
	return(dat < sn.dat);
}

int snode::cost(void)
{
	return(dat);   // dummy cost function = numerical value 
}



void snode::print(void)
{
	cout << dat;
}

snode::~snode()
{

}

ostream& operator<<(ostream& s, snode &sn) 
{
	return (s << sn.dat);
}



void lprint(list<snode> ls) {
	list<snode>::iterator p;
	for(p=ls.begin(); p!=ls.end(); p++) {
		cout << *p << " ";
	}
	cout << endl;
}




 
class sstate{

public:
	list<snode> remaining;
	list<snode> path;
	sstate();
	sstate(const sstate& ss);
	void operator=(const sstate& ss);
	bool operator<(sstate ss);
	int cost(void);
	list <sstate> expand();
	void print(void);
	bool is_solution(void);
	~sstate();
};

sstate::sstate()
{
	path.clear();
	remaining.clear();

}


sstate::sstate(const sstate& ss)
{
	path.clear();
	list<snode>::const_iterator p;
	for(p=ss.path.begin(); p!=ss.path.end(); p++) {
		path.push_back(*p);
	
	}

	remaining.clear();
	for(p=ss.remaining.begin(); p!=ss.remaining.end(); p++) {
		remaining.push_back(*p);        
	
	}

}

void sstate::operator=(const sstate& ss)
{
	path.clear();
	list<snode>::const_iterator p;
	for(p=ss.path.begin(); p!=ss.path.end(); p++) {
		path.push_back(*p);
	
	}

	remaining.clear();
	for(p=ss.remaining.begin(); p!=ss.remaining.end(); p++) {
		remaining.push_back(*p);        
	
	}
	
	
}


int sstate::cost(void)
{

	int res = 0;

	list<snode>::iterator p;
	for(p=path.begin(); p!=path.end(); p++) {
		res += p->cost();
	}
	return(res);



//	return(path.size());
}




bool sstate::operator<(sstate ss)
{
	return(cost() < ss.cost()); 
	return(path.size() < ss.path.size());

}



list<sstate> sstate::expand()
{

	
	
	list<snode> ru, rr, tr;
	// ru keeps track of those items on the remaining list that have been "used"
	// rr keeps track of those itmes on the remaining list that "remain" to be used
	// tr is a copy of rr that is used with the destructive splice function

	sstate ss;
	list<sstate> res;
	res.clear();
	
	if(remaining.size() == 0 )
		return(res);  // nothing left to do 


	ru.clear();
	rr=remaining;

	list<snode>::iterator q;

	//snode temp;
	q = rr.begin();
	while(q != rr.end()) {
		ss.path = path;
		ss.path.push_back(*q);

		ss.remaining=ru;

		tr = rr;
		tr.pop_front();
		//temp = *q;
		//rr.pop_front();

		
		ss.remaining.splice(ss.remaining.end(),tr);
		//for(list<snode>::iterator x = rr.begin(); x != rr.end(); x++) {
		//	ss.remaining.push_back(*x);
		//}

		ru.push_back(*q);
		//ru.push_back(temp);
		rr.pop_front();  // if not using tr method, comment this out
		
		res.push_back(ss);
		q=rr.begin();

	}
		
	return(res); 
}

void sstate::print(void)
{
	list<snode>::iterator p;
	cout << "[";
	for(p=path.begin(); p!=path.end(); p++) {
		cout << *p << " ";
	}
	cout << "] [" ;
	for(p=remaining.begin(); p!=remaining.end(); p++) {
		cout << *p << " ";
	}

	cout << "]" << endl; 

}

bool sstate::is_solution(void)
{

	if(path.size() != 4) return(0);
	return(1);  
}

sstate::~sstate()
{
	path.clear();
	remaining.clear();
}

double DepthFirstMatch::findBestDrawOrder(Definition *pcDef) {
	// Assume the first draw order is the best match
	//double dBestMatch = computeMatch(pcDef);
	//symbol()->saveBestDrawOrder(dBestMatch);
	// cout << "Initial match " << dBestMatch << endl;

	double dBestMatch = 1.0e6; 
	int numnodes;
	double MM;

	sstate a;
	list<sstate>::iterator qp;
	list<sstate> q, res, done;
	vector<int> nodeorder;
	list<snode>::iterator p;


	// initialize the first search state
	for(int i=0;i<symbol()->size();i++) {
		a.remaining.push_back(i);
	}

	// initialize the queue
	q.clear();
	done.clear();
	q.push_front(a);

	
	//cout << " ================================== \n";
	//cout << pcDef->name() << endl;
	// do the search
	qp = q.begin();
	while(qp != q.end()) {

		if(qp->path.size() == symbol()->size()){
			nodeorder.clear();
			for(p=qp->path.begin(); p!=qp->path.end(); p++) {
				int tdat = p->dat;
				if(tdat >= pcDef->NumofSegments()) tdat = -1;
				nodeorder.push_back(tdat);
			}
			/*
			nodeorder.clear();
			nodeorder.push_back(0);
			nodeorder.push_back(1);
			nodeorder.push_back(3);
			nodeorder.push_back(4);
			nodeorder.push_back(2);
			*/
			//symbol()->assignDrawOrder(nodeorder);
			symbol()->segindexmap = nodeorder;
			MM = computeMatch(pcDef);
			//cout << MM << endl;
			if(MM < dBestMatch) {
				dBestMatch = MM;
				symbol()->saveBestDrawOrder(dBestMatch);
				/*
				cout << "New best " << MM << endl;
				cout << symbol()->drawOrder() << endl;
				qp->print();
				*/
			}
			q.pop_front();
			qp=q.begin();

		} else {

			res = qp->expand();
			if(res.size() != 0) {
				q.pop_front();
				q.splice(q.begin(), res); 
				//q.sort();		// use the cost functions along with sort to do heuristic and optimal search
			} else {
				cout << "whoops **************************************\n";
				//done.push_back(*qp);  
									/* save all complete paths for exhaustive search
									if not doing exhaustive search, then remove this line */	
				
				q.pop_front();  // this line is necessary even if not doing exhaustive search
			}
			qp=q.begin();
		}
	}
	return(dBestMatch);
}
#endif

/*
//Modified from http://www.bearcave.com/random_hacks/permute.C

void swap(int *v, const int i, const int j) {
  int t;
  t = v[i];
  v[i] = v[j];
  v[j] = t;
}

void rotateLeft(int *v, const int start, const int n) {
  int tmp = v[start];
  for (int i = start; i < n-1; i++) {
    v[i] = v[i+1];
  }
  v[n-1] = tmp;
} // rotateLeft

static double dBestMatch = 0;

void permute(int *v, const int start, const int n,
			 Symbol *pcSymbol, Definition *pcDef) {
    if (v != 0) {
		vector<int> cOrder(n);
		for (int i = 0; i < n; i++) {
			if (v[i] >= pcDef->NumofSegments()) {
				cOrder[i] = -1;
			} else {
				cOrder[i] = v[i];
			}
		}
		pcSymbol->assignDrawOrder(cOrder);
		//symbol()->segindexmap = cOrder;
		double dMatch = pcDef->PercentMatch(pcSymbol);
		if (dMatch > dBestMatch) {
			dBestMatch = dMatch;
			pcSymbol->saveBestDrawOrder(dBestMatch);
		}
	}

	if (start < n) {
		int i, j;
		for (i = n-2; i >= start; i--) {
		for (j = i + 1; j < n; j++) {
			swap(v, i, j);
			permute(v, i+1, n, pcSymbol, pcDef);
		} // for j
		rotateLeft(v, i, n);
		} // for i
	}
} // permute

void init(int *v, int N) {
	for (int i = 0; i < N; i++) {
		v[i] = i;
	}
} // init

// Generic approach up to n = 8
double HybridMatch::findBestDrawOrder(Definition *pcDef) {
	int n = symbol()->size();
	if (n <= 8) {
		int *v = new int[n];
		init(v, n);
		dBestMatch = 0;
		permute(v, 0, n, symbol(), pcDef);
		delete []v;
	} else {
		dBestMatch = RandomMatch::findBestDrawOrder(pcDef);
	}
	return (dBestMatch);
}
*/

// Faster lookup approach up to n = 6
double HybridMatch::findBestDrawOrder(Definition *pcDef) {
	double dBestMatch = 0;
	if (symbol()->size() <= PERMUTATION_MAX) {
		Permutation cPermutation(symbol()->size());
		for (int i = 0; i < cPermutation.total(); i++) {
			incSwapCount();
			vector<int> v = cPermutation[i];
			for (int j = 0; j < v.size(); j++) {
				if (v[j] >= pcDef->NumofSegments()) {
					v[j] = -1;
				}
			}
			symbol()->assignDrawOrder(v);
			//symbol()->segindexmap = v;
			double dMatch = computeMatch(pcDef);
			if (dMatch > dBestMatch) {
				dBestMatch = dMatch;
				symbol()->saveBestDrawOrder(dBestMatch);
			}
		}
	} else {
		dBestMatch = RandomMatch::findBestDrawOrder(pcDef);
	}
	
	return (dBestMatch);
}
