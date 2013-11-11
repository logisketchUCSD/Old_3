// $Id: graph_based_doc.h,v 1.1 2005/06/22 23:05:42 weesan Exp $

// graph_based_doc.h : interface of the CGraphBasedDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_BASED_DOC_H__11B9A845_3ABB_4E14_920F_3F92033E3EC1__INCLUDED_)
#define AFX_GRAPH_BASED_DOC_H__11B9A845_3ABB_4E14_920F_3F92033E3EC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OpenGL View ClassDoc.h"

//class CGraphBasedDoc : public CDocument
class CGraphBasedDoc : public COpenGLViewClassDoc
{
protected: // create from serialization only
	CGraphBasedDoc();
	DECLARE_DYNCREATE(CGraphBasedDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphBasedDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphBasedDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraphBasedDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH_BASED_DOC_H__11B9A845_3ABB_4E14_920F_3F92033E3EC1__INCLUDED_)
