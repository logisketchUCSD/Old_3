// $Id: graph_based_view.h,v 1.7 2005/09/16 04:37:11 weesan Exp $

// graph_based_view.h : interface of the CGraphBasedView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_BASED_VIEW_H__89F01216_AC6B_43E1_9091_3B53101F64EE__INCLUDED_)
#define AFX_GRAPH_BASED_VIEW_H__89F01216_AC6B_43E1_9091_3B53101F64EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "COpenGLView.h"

#define WM_USER_MSG		(WM_APP + 1)

enum {
	WM_USER_CMD_SAVE,
	WM_USER_CMD_EXIT
};

//class CGraphBasedView : public CView
class CGraphBasedView : public COpenGLView
{
private:
	bool bMouseDown;

private:
	LRESULT CGraphBasedView::OnPenProximity(WPARAM hCtx, LPARAM l);
	void userMsgHandle(WPARAM wParam, LPARAM lParam);

protected: // create from serialization only
	CGraphBasedView();
	DECLARE_DYNCREATE(CGraphBasedView)

// Attributes
public:
	CGraphBasedDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphBasedView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphBasedView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Added by weesan@cs.ucr.edu
	bool save(const char *pzcFilename);

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraphBasedView)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in graph_based_view.cpp
inline CGraphBasedDoc* CGraphBasedView::GetDocument()
   { return (CGraphBasedDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH_BASED_VIEW_H__89F01216_AC6B_43E1_9091_3B53101F64EE__INCLUDED_)
