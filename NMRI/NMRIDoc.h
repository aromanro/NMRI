
// NMRIDoc.h : interface of the CNMRIDoc class
//


#pragma once


#include "NMRIFile.h"

class CNMRIDoc : public CDocument
{
protected: // create from serialization only
	CNMRIDoc() = default;
	DECLARE_DYNCREATE(CNMRIDoc)

// Attributes
public:
	NMRIFile theFile;

	bool animate = true;

	bool colorFunction = true;
	bool opacityFunction = true;
	bool gradientFunction = true;
	int opacityVal = 50;
	int gradientVal = 50;

// Operations
	bool Load(const CString& name);
	void UpdateViews();
	void Update3DOptions();

private:
// Overrides
	BOOL OnNewDocument() override;
	void Serialize(CArchive& ar) override;
#ifdef SHARED_HANDLERS
	void InitializeSearchContent() override;
	void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds) override;
#endif // SHARED_HANDLERS

// Implementation
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

// Generated message map functions
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
