// $Id: graph_based_doc.cpp,v 1.1 2005/06/22 23:05:42 weesan Exp $

// graph_based_doc.cpp : implementation of the CGraphBasedDoc class
//

#include "stdafx.h"
#include "graph_based.h"
#include "graph_based_doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedDoc

IMPLEMENT_DYNCREATE(CGraphBasedDoc, COpenGLViewClassDoc)

BEGIN_MESSAGE_MAP(CGraphBasedDoc, COpenGLViewClassDoc)
	//{{AFX_MSG_MAP(CGraphBasedDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedDoc construction/destruction

CGraphBasedDoc::CGraphBasedDoc()
{
	// TODO: add one-time construction code here
}

CGraphBasedDoc::~CGraphBasedDoc()
{
}

BOOL CGraphBasedDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGraphBasedDoc serialization

void CGraphBasedDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedDoc diagnostics

#ifdef _DEBUG
void CGraphBasedDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphBasedDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedDoc commands
