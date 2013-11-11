// $Id: graph_based_main.h,v 1.1 2005/06/22 23:05:42 weesan Exp $

// graph_based_main.h : interface of the CGraphBasedMain class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_BASED_MAIN_H__2816E047_466B_47BF_A55D_1333C38ACEE4__INCLUDED_)
#define AFX_GRAPH_BASED_MAIN_H__2816E047_466B_47BF_A55D_1333C38ACEE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraphBasedMain : public CFrameWnd
{
	
protected: // create from serialization only
	CGraphBasedMain();
	DECLARE_DYNCREATE(CGraphBasedMain)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphBasedMain)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphBasedMain();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CGraphBasedMain)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH_BASED_MAIN_H__2816E047_466B_47BF_A55D_1333C38ACEE4__INCLUDED_)
