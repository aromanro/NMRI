
// NMRIView.h : interface of the CNMRIView class
//

#pragma once


#include "MemoryBitmap.h"

class CNMRIView : public CView
{
protected: // create from serialization only
	CNMRIView() = default;
	DECLARE_DYNCREATE(CNMRIView)

// Attributes
public:
	CNMRIDoc* GetDocument() const;

// Operations
// Overrides
private:
	void OnDraw(CDC* pDC) override;  // overridden to draw this view
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

// Implementation
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

	MemoryBitmap img1;
	MemoryBitmap img2;

	int theFrame = 0;
	UINT_PTR timer = 0;

// Generated message map functions
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NMRIView.cpp
inline CNMRIDoc* CNMRIView::GetDocument() const
   { return reinterpret_cast<CNMRIDoc*>(m_pDocument); }
#endif

