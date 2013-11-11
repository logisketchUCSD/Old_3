// $Id: graph_based_view.cpp,v 1.23 2007/04/17 23:45:20 weesan Exp $

// graph_based_view.cpp : implementation of the CGraphBasedView class
//

#include "stdafx.h"
#include "graph_based.h"

#include "graph_based_doc.h"
#include "graph_based_view.h"

#include "extern.h"
#include "glue.h"
#include "definition.h"
#include "recognizer.h"
#include "def_viewer.h"
#include "options.h"
#include "training.h"
#include "debug.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedView

IMPLEMENT_DYNCREATE(CGraphBasedView, COpenGLView)

BEGIN_MESSAGE_MAP(CGraphBasedView, COpenGLView)
	//{{AFX_MSG_MAP(CGraphBasedView)
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	// Handle Tablet WT_PROXIMITY message
	ON_MESSAGE(WT_PROXIMITY, OnPenProximity)
	//ON_MESSAGE(WM_USER_MSG, userMsgHandle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedView construction/destruction

#include <Fl/Fl.h>
CGraphBasedView::CGraphBasedView() : bMouseDown(false)
{
	// TODO: add construction code here
	if (cOptions.trainingMode()) {
		// XXX: Doesn't work
		// Position the drawing window
		//SetWindowPos(&wndTopMost, 512, 0, 512, 512, SWP_SHOWWINDOW);

		// Turn on the raw ink mode only
		show_ink = 1;
		show_seg = 0;
		show_seg_point = 0;
		// Since the thread will delete itself after it's done, so, it needs
		// to be allocated.
#ifdef HAVE_FLTK
		Training *pcTraining = new Training(this,
											cOptions.user(),
											cOptions.testSamples(),
											cOptions.trainingSamples(),
											cOptions.pause());
		assert(pcTraining != NULL);
		pcTraining->start();
#else
		fprintf(stderr, "Warning: no FLTK!\n");
#endif
	}
}

CGraphBasedView::~CGraphBasedView()
{
}

BOOL CGraphBasedView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedView drawing

void CGraphBasedView::OnDraw(CDC* pDC)
{
	CGraphBasedDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	pDoc->SetTitle(glue_sketches_title(CurrentSketch, &Sketches));
	COpenGLView::OnDraw(pDC);
	// Only display segment no when not in training mode
	if (!cOptions.trainingMode()) {
		glue_display_segment_no();
		// XXX: Commented out because the screen is screwed
		//::SwapBuffers(pDC->GetSafeHdc());
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedView diagnostics

#ifdef _DEBUG
void CGraphBasedView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphBasedView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphBasedDoc* CGraphBasedView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphBasedDoc)));
	return (CGraphBasedDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGraphBasedView message handlers

void CGraphBasedView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar) {
	case 'G':  // Switch 'T' to 'G' for toggle
		show_ink = 1 - show_ink;
		show_seg = 1 - show_ink;
		show_seg_ends = 0;
		show_seg_point = 0;
		PostMessage(WM_PAINT);
		return;
		break;
	case 'T':  // Train the recognizer
		if (glue_sketches_empty(&Sketches)) {
			DEBUG("Nothing to be trained!");
		} else {
			glue_train(this, &Sketches);
		}
		// Return here instead of break to override parent's keystroke
		return;
	case 'R':
		if (glue_sketch_empty(CurrentSketch)) {
			DEBUG("Nothing to be recognized!");
		} else {
			glue_recognize(CurrentSketch);
		}
		break;
	case VK_DELETE:
		if (glue_sketches_empty(&Sketches)) {
			DEBUG("No more sketch to be deleted!");
		} 
		break;
	case 'I':
		::Beep(1000, 100);
		PostMessage(WM_USER_MSG, WM_USER_CMD_SAVE, (LPARAM)"This is a file.txt");
		break;
	case 'B':
		system("\"..\\3dpen-v3-tablet-ver3\\Debug\\Opengl view class.exe\"");
		break;
	case 'F': {
		Symbol *pcSymbol = glue_new_symbol(CurrentSketch);
		if (pcSymbol) {
			cout << *pcSymbol;
			delete pcSymbol;
		}
		return;
	}
	case 'V': {
#ifdef HAVE_FLTK
		DefsViewer cDefsViewer;
		cDefsViewer.run();
#else
		fprintf(stderr, "Warning: no FLTK!\n");
#endif
		return;
	}
	case 'C':
		if (glue_sketches_empty(&Sketches)) {
			DEBUG("Nothing to compare!");
		} else {
			glue_compare(&Sketches);
		}
		return;
	case 'W':
		if (glue_sketches_empty(&Sketches)) {
			DEBUG("Nothing to answer!");
		} else {
			glue_answer(&Sketches);
		}
		return;
	case ' ':
		// Only when in training, SPC is treat as advance to next symbol
#ifdef HAVE_FLTK
		if (cOptions.trainingMode()) {
			training_next_sym();
			return;
		}
#endif
		break;
	default:
		break;
	}

	COpenGLView::OnKeyUp(nChar, nRepCnt, nFlags);
}

/*
// Have to override this to restore MFC's original function
void CGraphBasedView::OnFileSave() 
{
	// TODO: Add your command handler code here
	//GetDocument()->SetModifiedFlag(TRUE);
	//GetDocument()->SetPathName("definitions\\*.txt", true);
	//GetDocument()->DoFileSave();
	
	CFileDialog d(TRUE, NULL, NULL, NULL,
				  "Text files (*.txt)|*.txt|All Files (*.*)|*.*||");
	if (d.DoModal() == IDOK) {
		fstream ofs(d.GetPathName(), ios::out | ios::app);
		if (!ofs) {
			DEBUG("Failed to open %s", d.GetPathName());
		} else {
			//AfxGetApp()->AddToRecentFileList(d.GetPathName());
			Symbol *pcSymbol = glue_new_symbol(CurrentSketch);
			ofs << *pcSymbol;
			delete pcSymbol;
		}
	}
	
}

// Have to override this to restore MFC's original function
void CGraphBasedView::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	OnFileSave();
}

// Have to override this to restore MFC's original function
void CGraphBasedView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	//((CGraphBasedApp *)AfxGetApp())->doFileOpen();
	CFileDialog d(TRUE, NULL, NULL, NULL,
				  "Text files (*.txt)|*.txt|All Files (*.*)|*.*||");
	if (d.DoModal() == IDOK) {
		//printf("[%s]\n", d.GetPathName());
		fstream ifs(d.GetPathName(), ios::in);
		if (!ifs) {
			DEBUG("Failed to open %s", d.GetPathName());
		} else {
			//AfxGetApp()->AddToRecentFileList(d.GetPathName());
			int i = 0;
			while (ifs.peek() != -1) {
				printf("\n==> Symbol %d\n", ++i);
				Symbol *pcSymbol = new Symbol;
				assert(pcSymbol != NULL);
				// Read the symbol
				ifs >> *pcSymbol;
				// Process it
				pcSymbol->Process();
				// Recognize it
				recognizer_match(pcSymbol);
				delete pcSymbol;
			}
		}
	}
}
*/

bool CGraphBasedView::save(const char *pzcFilename) {
	// Check if there is any sketch to be saved
	if (glue_sketch_empty(CurrentSketch))  {
		DEBUG("Nothing to be saved!");
		return (false);
	}

	// Append data to the file if one exists
	ofstream ofs(pzcFilename, ios::app);
	if(!ofs) {
		char pzcBuffer[256];
		snprintf(pzcBuffer, sizeof(pzcBuffer),
				 "Could not open %s", pzcFilename);
		MessageBox(pzcBuffer);
		return (false);
	} else {
		CurrentSketch->Write(ofs);
		ofs.close();
		AfxGetApp()->AddToRecentFileList(pzcFilename);
		return (true);
	}
}

void CGraphBasedView::OnFileSave() 
{
	// Do not prompt for overwritten since new data will be appended
	CFileDialog m_ldFile(FALSE, "txt", "trace", !OFN_OVERWRITEPROMPT);

	if (m_ldFile.DoModal()  == IDOK) {
		save(m_ldFile.GetPathName());
	}
}

void CGraphBasedView::OnFileSaveAs() 
{
	OnFileSave();	
}

void CGraphBasedView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CFileDialog d(TRUE, NULL, NULL, NULL,
				  "Text files (*.txt)|*.txt|All Files (*.*)|*.*||");
	if (d.DoModal() == IDOK) {
		//printf("[%s]\n", d.GetPathName());
		if (!glue_read_raw_data(d.GetPathName())) {
			DEBUG("Failed to read %s", d.GetPathName());
		}
	}
}

void CGraphBasedView::OnLButtonDown(UINT nFlags, CPoint point) 
{
/*
	bMouseDown = true;

	CurrentSketch->Strokes.InsertEnd(new Stroke(MAX_PTS));
	stroke_data = CurrentSketch->Strokes[CurrentSketch->Strokes.GetSize()-1];
	stroke_data->EndData=0;
	stroke_data->seglist.Clear();
	assert(stroke_data != NULL);
*/
	COpenGLView::OnLButtonDown(nFlags, point);
}

void CGraphBasedView::OnMouseMove(UINT nFlags, CPoint point) 
{
/*
	// Display segments' coordinates
	CRect r; 
	GetClientRect(&r);
	CPoint p(0,0);
	ClientToScreen(&p);

	int x = p.x + point.x;
#ifdef HIGH_RES
	int y = 1024 - p.y - point.y;
#else
	int y = 768 - p.y - point.y;
#endif
	glue_display_segment_coord(x, y);

	if (nFlags & MK_LBUTTON) {
		stroke_data->AddPoint(point.x, 768 - point.y, 0, 0, 0, GetTickCount());
	} else {
		bMouseDown = false;
	}
	InvalidateRect(0,FALSE);
	GetParent()->PostMessage(WM_PAINT);
*/
	PostMessage(WM_SETFOCUS);
	COpenGLView::OnMouseMove(nFlags, point);
}

void CGraphBasedView::OnLButtonUp(UINT nFlags, CPoint point) 
{
/*
	bMouseDown = false;
*/
	COpenGLView::OnLButtonUp(nFlags, point);
}

void CGraphBasedView::userMsgHandle(WPARAM wParam, LPARAM lParam) {
	printf("Hello: %d, [%s]\n", wParam, lParam);
}

LRESULT CGraphBasedView::OnPenProximity(WPARAM hCtx, LPARAM l) {
	if (!LOBYTE(l)) {
		// Lost focus
		PostMessage(WM_KILLFOCUS);
	}
	return (TRUE);
}

void CGraphBasedView::OnKillFocus(CWnd* pNewWnd) 
{
	COpenGLView::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here

	//TABLET:  restrict detection to only while it has the input focus
	if(tabletHandle()){
		WTEnable(tabletHandle(), FALSE);	// disable packet detection
	}
}

void CGraphBasedView::OnSetFocus(CWnd* pOldWnd) 
{
	COpenGLView::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here

	//TABLET: restrict detection to only while it has the input focus
	if(tabletHandle()){
		WTEnable(tabletHandle(), TRUE);	// enable packet detection
	}
}
