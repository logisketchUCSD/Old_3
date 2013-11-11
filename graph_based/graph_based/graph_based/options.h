// $Id: options.h,v 1.8 2006/12/23 08:37:40 weesan Exp $

/*
 * options.h - Options from commandline.
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	8/14/2005
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "match.h"
#include "definition.h"
#include "debug.h"

#define OPTIONS_STRING(name, len)											\
	private:																\
		char _##name[len];													\
	public:																	\
		const char *name(void) const {										\
			if (_##name[0]) {												\
				return (_##name);											\
			} else {														\
				return (NULL);												\
			}																\
		}																	\
		void name(const char *_##name) {									\
			snprintf(this->_##name, sizeof(this->_##name), "%s", _##name);	\
		}

#define OPTIONS_TYPE(name, type)											\
	private:																\
		type _##name;														\
	public:																	\
		const type name(void) const {										\
			return (_##name);												\
		}																	\
		void name(type _##name) {											\
			this->_##name = _##name;										\
		}


class Options {
	OPTIONS_STRING(defDir, _MAX_PATH);
	OPTIONS_STRING(symbolDir, _MAX_PATH);
	OPTIONS_STRING(assignOrder, 256);
	OPTIONS_STRING(outDir, _MAX_PATH);
	OPTIONS_STRING(trainingMode, 256);
	OPTIONS_STRING(validation, 256);
	OPTIONS_TYPE(match, Match::Type);
	OPTIONS_TYPE(defsType, Definitions::Type);
	OPTIONS_TYPE(bestN, int);
	OPTIONS_TYPE(valueN, int);
	OPTIONS_TYPE(verbose, int);
	OPTIONS_TYPE(wrapper, bool);
	OPTIONS_TYPE(convert, bool);
	OPTIONS_TYPE(random, int);
	OPTIONS_TYPE(plot, bool);
	OPTIONS_TYPE(iteration, int);

private:
	// For training
	char pzcUser[64];
	int iTestSamples;
	int iTrainingSamples;
	int iPause;
	// For k-fold validation
	int iUserNo;
	int iFromMatcher, iToMatcher;
	int iFromTraining, iToTraining;
	int iFromRun, iToRun;

public:
	Options(void) :
		iTestSamples(0),
		iTrainingSamples(0),
		iPause(0),
		iUserNo(0),
		iFromMatcher(0),
		iToMatcher(0),
		iFromTraining(0),
		iToTraining(0),
		iFromRun(0),
		iToRun(0) {
		pzcUser[0] = 0;
	}
	void parseTrainingMode(const char *pzcTraining) {
		// Store the training mode string
		trainingMode(pzcTraining);

		int n = sscanf(_trainingMode, "%[^:]:%d+%d/%d",
					   pzcUser, &iTestSamples, &iTrainingSamples, &iPause);
		assert(n >= 3);
	}
	const char *user(void) const {
		return (pzcUser);
	}
	int testSamples(void) const {
		return (iTestSamples);
	}
	int trainingSamples(void) const {
		return (iTrainingSamples);
	}
	int pause(void) const {
		return (iPause);
	}
	void parseValidation(const char *pzcOption);
	int userNo(void) const {
		return (iUserNo);
	}
	int fromMatcher(void) const {
		return (iFromMatcher);
	}
	int toMatcher(void) const {
		return (iToMatcher);
	}
	int fromTraining(void) const {
		return (iFromTraining);
	}
	int toTraining(void) const {
		return (iToTraining);
	}
	int fromRun(void) const {
		return (iFromRun);
	}
	int toRun(void) const {
		return (iToRun);
	}
};

extern Options cOptions;

#endif // OPTIONS_H