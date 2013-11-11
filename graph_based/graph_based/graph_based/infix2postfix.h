// $Id: infix2postfix.h,v 1.1 2006/02/10 22:12:47 weesan Exp $

/*
 * infix2postfix.h - Covert Infix notation to Postfix notation and evaluate it
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	1/17/2006
 */

#ifndef INFIX2POSTFIX_H
#define INFIX2POSTFIX_H

#include <stdlib.h>
#include <stack>

using namespace std;

class Infix2Postfix {
private:
	char *pzcInfix;
	bool bOk;
	stack<double> cOperands;
	stack<int> cOps;

private:
	void process_operands(int op);
	void process_operator(int op);

public:
	Infix2Postfix(const char *pzcInfix);
	~Infix2Postfix(void) {
		if (pzcInfix) {
			free(pzcInfix);
		}
	}
	char *infix(void) const {
		return (pzcInfix);
	}
	bool ok(void) const {
		return (bOk);
	}
	double eval(void);
};

#endif // INFIX2POSTFIX_H