
// NMRIDoc.h : interface of the CNMRIDoc class
//


#pragma once


#include "NMRIFile.h"

class CNMRIDoc : public CDocument
{
protected: // create from serialization only
	CNMRIDoc();
	DECLARE_DYNCREATE(CNMRIDoc)

// Attributes
public:
	NMRIFile theFile;

	bool animate;

// Operations
public:
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CNMRIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	bool Load(const CString& name);
};
