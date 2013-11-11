// $Id: utils.h,v 1.1 2005/06/22 23:05:42 weesan Exp $

/*
 * utils.h - Some simple utilities like input dialog and etc
 *
 * Author:		WeeSan Lee <weesan@cs.ucr.edu>
 * Created on:	5/17/2005
 */

#ifndef UTILS_H
#define UTILS_H

#include "stdafx.h"

class InputLine : public CDialog {
private:
	CEdit *pcEdit;

public:
	InputLine(const char *pzcPrompt) : CDialog("") {
		pcEdit = new CEdit;
		pcEdit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
			CRect(10, 10, 100, 100), this, 1);
	}
	BOOL OnInitDialog(void) {
		CDialog::OnInitDialog();
		pcEdit->SetWindowText("My Name");
		return (TRUE);
	}
};

#endif // UTILS_H
