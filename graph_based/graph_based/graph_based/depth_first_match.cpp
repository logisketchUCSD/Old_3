#include "depth_first_match.h"
#include "definition.h"

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

