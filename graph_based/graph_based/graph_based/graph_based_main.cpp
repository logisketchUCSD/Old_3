// $Id: graph_based_main.cpp,v 1.2 2005/08/20 18:31:13 weesan Exp $

// graph_based_main.cpp : implementation of the CGraphBasedMain class
//

#include "stdafx.h"
#include "graph_based.h"

#include "graph_based_main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedMain

IMPLEMENT_DYNCREATE(CGraphBasedMain, CFrameWnd)

BEGIN_MESSAGE_MAP(CGraphBasedMain, CFrameWnd)
	//{{AFX_MSG_MAP(CGraphBasedMain)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedMain construction/destruction

CGraphBasedMain::CGraphBasedMain()
{
	// TODO: add member initialization code here
}

CGraphBasedMain::~CGraphBasedMain()
{
}

int CGraphBasedMain::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CGraphBasedMain::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedMain diagnostics

#ifdef _DEBUG
void CGraphBasedMain::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CGraphBasedMain::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedMain message handlers

