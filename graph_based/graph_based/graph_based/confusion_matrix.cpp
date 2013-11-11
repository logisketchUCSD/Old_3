// $Id: confusion_matrix.cpp,v 1.1 2006/05/19 00:46:08 weesan Exp $

/*
 * confusion_matrix.cpp - A confusion matrix implementation.
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	5/16/2006
 */

#include "confusion_matrix.h"
#include "definition.h"

void ConfusionMatrix::dump(FILE *fp) {
	// Display the confusion matrix
	fprintf(fp, "Confusion matrix:\n");
	fprintf(fp, "      ");
	int i;
	for (i = 0; i < cDefs.size(); i++) {
		fprintf(fp, "%.2s ", (LPCTSTR)cDefs[i]->name());
	}
	fprintf(fp, "\n");

	for (i = 0; i < cDefs.size(); i++) {
		const char *pzcRow = (LPCTSTR)cDefs[i]->name();
		fprintf(fp, "%-4.4s ", pzcRow);
		for (int j = 0; j < cDefs.size(); j++) {
			const char *pzcCol = (LPCTSTR)cDefs[j]->name();
			fprintf(fp, "%2d ", (*this)[pzcRow][pzcCol]);
			/*
			// More space efficient this way
			if (cConfMatrix[pzcRow].find(pzcCol) != cConfMatrix[pzcRow].end()) {
				fprintf(fp, "%2d ", cConfMatrix[pzcRow][pzcCol]);
			} else {
				fprintf(fp, "%2d ", 0);
			}
			*/
		}
		fprintf(fp, "\n");
	}
}
