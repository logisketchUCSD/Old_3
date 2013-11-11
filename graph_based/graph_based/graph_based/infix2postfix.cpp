// $Id: infix2postfix.cpp,v 1.3 2007/01/22 22:25:54 weesan Exp $

/*
 * infix2postfix.cpp - Covert Infix notation to Postfix notation and evaluate it
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	1/17/2006
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "infix2postfix.h"

#define LP	1
#define RP	2
#define ADD	3
#define SUB	4
#define MUL	5
#define	DIV	6
#define POW	7

Infix2Postfix::Infix2Postfix(const char *pzcInfix) :
	pzcInfix(strdup(pzcInfix)),
	bOk(true) {
	assert(pzcInfix != NULL);
}

void Infix2Postfix::process_operands(int op) {
	double d2 = cOperands.top();
	cOperands.pop();
	double d1 = cOperands.top();
	cOperands.pop();

	double dResult = 0;
	switch (op) {
	case ADD:
		dResult = d1 + d2;
		break;
	case SUB:
		dResult = d1 - d2;
		break;
	case MUL:
		dResult = d1 * d2;
		break;
	case DIV:
		if (d2 == 0) {
			bOk = false;
		} else {
			dResult = d1 / d2;
		}
		break;
	default:
		fprintf(stderr, "Unknown operator: %d\n", op);
		break;
	}

	cOperands.push(dResult);
}

void Infix2Postfix::process_operator(int op) {
	switch (op) {
	case LP:
		// Do nothing but push '(' into operator stack
		cOps.push(op);
		break;
	case RP:
		// Pop and compute every operator and corresponding operands
		// till the closet '(' is met
		while (!cOps.empty() && cOps.top() != LP) {
			process_operands(cOps.top());
			cOps.pop();
		}
		if (cOps.empty()) {
			// Shouldn't empty
			fprintf(stderr, "Error: missing left parenthesis!\n");
		} else {
			// Pop out the '(' from the stack
			cOps.pop();
		}
		break;
	default:
		// Pop and compute operators and operands with higher operator 
		// precedence.  For example, if '*' is in the op stack, pop it out
		// before pushing in '+'
		while (!cOps.empty() && op <= cOps.top()) {
			process_operands(cOps.top());
			cOps.pop();
		}
		cOps.push(op);
	}
}

double Infix2Postfix::eval(void) {
	const char *delim = " \t\n";
	for (const char *p = strtok(infix(), delim); p; p = strtok(NULL, delim)) {
		switch (*p) {
		case '+':
			process_operator(ADD);
			break;
		case '-':
			process_operator(SUB);
			break;
		case '*':
			process_operator(MUL);
			break;
		case '/':
			process_operator(DIV);
			break;
		case '(':
			process_operator(LP);
			break;
		case ')':
			process_operator(RP);
			break;
		default:
			if (isdigit(*p)) {
				cOperands.push(atof(p));
			} else {
				fprintf(stderr, "Invalid token: %d\n", *p);
			}
			break;
		}
	}

	while (!cOps.empty()) {
		process_operands(cOps.top());
		cOps.pop();
	}

	return (cOperands.top());
}

#ifdef DEBUG_INFIX2POSTFIX
int main(void) {
	//Infix2Postfix i2p("( 1 + 2 ) * ( ( 2 + 3 ) * 2 )");
	//Infix2Postfix i2p("1 + 2 * 3");
	//Infix2Postfix i2p("1 * 2 * 3 * 4");
	Infix2Postfix i2p("0 - 0 * ( 1 - 0 ) + 1");
	//Infix2Postfix i2p("0 - 0 + 1");
	printf("Ans = %.02f\n", i2p.eval());
}
#endif
