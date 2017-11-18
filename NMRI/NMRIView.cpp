
// NMRIView.cpp : implementation of the CNMRIView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "NMRI.h"
#endif

#include "NMRIDoc.h"
#include "NMRIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNMRIView

IMPLEMENT_DYNCREATE(CNMRIView, CView)

BEGIN_MESSAGE_MAP(CNMRIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CNMRIView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CNMRIView construction/destruction

CNMRIView::CNMRIView()
	: theFrame(0), timer(NULL)
{
	// TODO: add construction code here

}

CNMRIView::~CNMRIView()
{
}

BOOL CNMRIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CNMRIView drawing

void CNMRIView::OnDraw(CDC* pDC)
{
	const CNMRIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	int width = pDoc->theFile.Width;
	int height = pDoc->theFile.Height;
	
	img1.SetMatrix(pDoc->theFile.GetFrame(), width, height);
	img2.SetMatrix(pDoc->theFile.GetRealFrame(), width, height);

	CRect rect;
	GetClientRect(rect);

	width *= 2;
	height *= 2;


	CRect rct;
	rct.top = rect.top + rect.Height() / 2 - height / 2;
	rct.left = rect.left + rect.Width() / 2 - width - 5;
	rct.right = rct.left + width;
	rct.bottom = rct.top + height;

	img1.Draw(pDC, rct);

	rct.left = rect.left + rect.Width() / 2 + 5;
	rct.right = rct.left + width;

	img2.Draw(pDC, rct);


	CBrush whiteBrush(RGB(255,255,255));
	CRect paintRect;

	paintRect.top = rect.top;
	paintRect.bottom = rct.top;
	paintRect.left = rect.left;
	paintRect.right = rect.right;
	pDC->FillRect(paintRect, &whiteBrush);

	paintRect.top = rct.bottom;
	paintRect.bottom = rect.bottom;
	pDC->FillRect(paintRect, &whiteBrush);

	paintRect.right = rect.left + rect.Width() / 2 - width - 5;
	paintRect.top = rct.top;
	paintRect.bottom = rct.bottom;
	pDC->FillRect(paintRect, &whiteBrush);

	paintRect.left = rct.right;
	paintRect.right = rect.right;
	pDC->FillRect(paintRect, &whiteBrush);

	paintRect.left = rct.left-10;
	paintRect.right = rct.left;
	pDC->FillRect(paintRect, &whiteBrush);
}


// CNMRIView printing


void CNMRIView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CNMRIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNMRIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNMRIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CNMRIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CNMRIView::OnContextMenu(CWnd* /* pWnd */, CPoint /*point*/)
{
#ifndef SHARED_HANDLERS
#endif
}


// CNMRIView diagnostics

#ifdef _DEBUG
void CNMRIView::AssertValid() const
{
	CView::AssertValid();
}

void CNMRIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNMRIDoc* CNMRIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNMRIDoc)));
	return dynamic_cast<CNMRIDoc*>(m_pDocument);
}
#endif //_DEBUG


// CNMRIView message handlers


int CNMRIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	timer = SetTimer(1, 500, NULL);

	return 0;
}


void CNMRIView::OnDestroy()
{
	KillTimer(timer);

	CView::OnDestroy();
}


BOOL CNMRIView::OnEraseBkgnd(CDC* pDC)
{
	if (pDC->IsPrinting())
		return CView::OnEraseBkgnd(pDC);

	return TRUE;
}


void CNMRIView::OnTimer(UINT_PTR nIDEvent)
{
	CView::OnTimer(nIDEvent);


	CNMRIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	++theFrame;

	if (theFrame >= pDoc->theFile.NrFrames) theFrame = 0;

	pDoc->theFile.FFT(theFrame);

	Invalidate();
}
