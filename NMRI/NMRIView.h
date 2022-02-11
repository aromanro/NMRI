
// NMRIView.h : interface of the CNMRIView class
//

#pragma once


#include "MemoryBitmap.h"

class CNMRIView : public CView
{
protected: // create from serialization only
	CNMRIView();
	DECLARE_DYNCREATE(CNMRIView)

// Attributes
public:
	CNMRIDoc* GetDocument() const;

// Operations
public:
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CNMRIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	MemoryBitmap img1;
	MemoryBitmap img2;

	int theFrame;
	UINT_PTR timer;

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in NMRIView.cpp
inline CNMRIDoc* CNMRIView::GetDocument() const
   { return reinterpret_cast<CNMRIDoc*>(m_pDocument); }
#endif

