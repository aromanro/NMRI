
// NMRIDoc.cpp : implementation of the CNMRIDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "NMRI.h"
#endif

#include "NMRIDoc.h"

#include "NMRIFile.h"
#include "MainFrm.h"

#include "VTKView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNMRIDoc

IMPLEMENT_DYNCREATE(CNMRIDoc, CDocument)

BEGIN_MESSAGE_MAP(CNMRIDoc, CDocument)
END_MESSAGE_MAP()


// CNMRIDoc construction/destruction

BOOL CNMRIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	if (Load(L"Head2D.dat"))
		SetTitle(L"Head2D");
	else
		SetTitle(L"No file loaded");
	
	return TRUE;
}




// CNMRIDoc serialization

void CNMRIDoc::Serialize(CArchive& ar)
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

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CNMRIDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CNMRIDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CNMRIDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CNMRIDoc diagnostics

#ifdef _DEBUG
void CNMRIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNMRIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNMRIDoc commands


bool CNMRIDoc::Load(const CString& name)
{
	if (theFile.Load(name))
	{
		theFile.FFT(0);

		return true;
	}

	return false;
}


void CNMRIDoc::UpdateViews()
{
	POSITION pos = GetFirstViewPosition();
	while (pos != nullptr) {
		CView* pView = GetNextView(pos);
		
		// the other one refreshes itself by timer
		if (pView->IsKindOf(RUNTIME_CLASS(CVTKView)))
		{
			((CVTKView*)pView)->GrabResultsFromDoc();
			((CVTKView*)pView)->Invalidate();
		}
	}
}


void CNMRIDoc::Update3DOptions()
{
	POSITION pos = GetFirstViewPosition();
	while (pos != nullptr) {
		CView* pView = GetNextView(pos);

		// the other one refreshes itself by timer
		if (pView->IsKindOf(RUNTIME_CLASS(CVTKView)))
		{
			((CVTKView*)pView)->UpdateTransferFunctions();
			((CVTKView*)pView)->Invalidate();
		}
	}
}