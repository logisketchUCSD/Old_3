
%{

// $Id: split.l,v 1.2 2005/11/21 22:52:08 weesan Exp $

/*
 * split.l - Split Leslie's user data into defintions and symbols.
 *
 * Author:      WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:  11/8/05
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#define SPLIT_TRAINING_COUNT    7

using namespace std;

vector<string> cSymbols;
map<string, int> cCount;
int iDef = 0;
int iSym = 0;
char pzcDefDir[256];
char pzcSymDir[256];

%}

%x NAME
%x BODY

%%

^[tr] {
    BEGIN NAME;
}

^\r\n {
    // Skip blank lines
}

^[0-9]+\r\n {
    // This is fixing the problem of having 1 less than the real # of stroke :(
    int iStrokeNo = 0;
    if (sscanf(yytext, "%d", &iStrokeNo) != 1) {
	fprintf(stderr, "Failed to get # of stroke\n");
	exit(1);
    } else if (iStrokeNo <= 0) {
	fprintf(stderr, "Error: # of stroke can't be equal or less than 0\n");
	exit(1);
    } else {
	char buf[256];
	snprintf(buf, sizeof(buf), "%d\r\n", iStrokeNo - 1);
	cSymbols.push_back(buf);
    }
    BEGIN BODY;
}

.|\r\n {
    printf("Unknown symbol: %s\n", yytext);
}

<NAME>[ ]+ {
    // Skip spaces
}

<NAME>[a-z]* {
    char pzcFilename[256], *pzcName = yytext;

    // Rename some symbol names
    switch (*yytext) {
    case 'i':
	if (strcmp(yytext, "intergral") == 0) {
	    pzcName = "integral";
	}
	break;
    case 'p':
	if (strcmp(yytext, "poundsign") == 0) {
	    pzcName = "pound";
	}
	break;
    case 's':
	if (strcmp(yytext, "sqrroot") == 0) {
	    pzcName = "sqrt";
	}
	break;
    default:
	break;
    }

    cCount[pzcName]++;

    if (cCount[pzcName] < SPLIT_TRAINING_COUNT) {
	iDef++;
	snprintf(pzcFilename, sizeof(pzcFilename), "%s/%s.txt",
		 pzcDefDir, pzcName);
    } else {
	iSym++;
	snprintf(pzcFilename, sizeof(pzcFilename), "%s/%s.txt",
		 pzcSymDir, pzcName);
    }

    ofstream ofs(pzcFilename, ios::out | ios::app);
    for (int i = 0; i < cSymbols.size(); i++) {
	ofs << cSymbols[i];
    }
    printf("Saved data into symbols %s\n", pzcFilename);
    cSymbols.clear();
}

<NAME>\r\n {
    BEGIN INITIAL;
}

<BODY>^\r\n {
    if (yyleng != 2) {
	// It has reached a full length of a symbol
	yyless(yyleng - 2);
	cSymbols.push_back(yytext);
    }
    BEGIN INITIAL;
}

<BODY>.|\r\n {
    // Push it back to the input
    yymore();
}

%%

int yywrap(void) {
   return 1;
}

void createDir(int iUserNo = 0) {
    if (iUserNo > 0) {
	snprintf(pzcDefDir, sizeof(pzcDefDir), "definitions.user%d", iUserNo);
    } else {
	snprintf(pzcDefDir, sizeof(pzcDefDir), "definitions");
    }

    if (mkdir(pzcDefDir, 0755) < 0) {
	exit(1);
    }

    if (iUserNo > 0) {
	snprintf(pzcSymDir, sizeof(pzcSymDir), "symbols.user%d", iUserNo);
    } else {
	snprintf(pzcSymDir, sizeof(pzcSymDir), "symbols");
    }

    if (mkdir(pzcSymDir, 0755) < 0) {
	exit(1);
    }
}

int main(int argc, char *argv[]) {
    // Create definitions and symbols first
    if (argc == 2) {
	createDir(atoi(argv[1]));
    } else {
	createDir();
    }

    // Start parsing
    yylex();

    printf("%d definitions, %d symbols, %d total converted!\n",
	   iDef, iSym, iDef + iSym);
}
