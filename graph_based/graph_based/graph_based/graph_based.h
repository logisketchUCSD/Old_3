// $Id: graph_based.h,v 1.2 2005/08/20 18:31:13 weesan Exp $



// graph_based.h : main header file for the GRAPH_BASED application

//



#if !defined(AFX_GRAPH_BASED_H__8D5ECB78_B157_4D64_9BEE_73811A16C95C__INCLUDED_)

#define AFX_GRAPH_BASED_H__8D5ECB78_B157_4D64_9BEE_73811A16C95C__INCLUDED_



#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000



#ifndef __AFXWIN_H__

	#error include 'stdafx.h' before including this file for PCH

#endif



#include "resource.h"       // main symbols

#include "Socket.h"



#define GRAPH_BASED_SERVER_PORT		1234



/////////////////////////////////////////////////////////////////////////////

// CGraphBasedApp:

// See graph_based.cpp for the implementation of this class

//



class CGraphBasedApp : public CWinApp, public Socket

{

public:

	CGraphBasedApp();



// Overrides

	// ClassWizard generated virtual function overrides

	//{{AFX_VIRTUAL(CGraphBasedApp)

	public:

	virtual BOOL InitInstance();

	//}}AFX_VIRTUAL



// Implementation

	//{{AFX_MSG(CGraphBasedApp)

	afx_msg void OnAppAbout();

	//}}AFX_MSG

	// Added by weesan@cs.ucr.edu

	/*

	void doFileOpen(void) {

		OnFileOpen();

	}

	*/

	DECLARE_MESSAGE_MAP()

};





/////////////////////////////////////////////////////////////////////////////



//{{AFX_INSERT_LOCATION}}

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_GRAPH_BASED_H__8D5ECB78_B157_4D64_9BEE_73811A16C95C__INCLUDED_)

