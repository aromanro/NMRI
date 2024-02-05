
#pragma once

class CNMRIDoc;


// for CSliderProp and CPropSliderCtrl see: https://github.com/jhlee8804/MFC-Feature-Pack/tree/master/NewControls
// modified to allow setting a range

class CSliderProp : public CMFCPropertyGridProperty
{
public:
	CSliderProp(const CString& strName, long nValue, long minVal = 0, long maxVal = 100, LPCTSTR lpszDescr = nullptr, DWORD dwData = 0);

	BOOL OnUpdateValue() override;

private:
	CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat) override;
	BOOL OnSetCursor() const  override { return FALSE; /* Use default */ }

	long m_minVal;
	long m_maxVal;
};

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl window

class CPropSliderCtrl : public CSliderCtrl
{
	// Construction
public:
	CPropSliderCtrl(CSliderProp* pProp, COLORREF clrBack);

	// Attributes
private:
	CBrush m_brBackground;
	COLORREF m_clrBack;
	CSliderProp* m_pProp;

	// Implementation

	//{{AFX_MSG(CPropSliderCtrl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void HScroll(UINT nSBCode, UINT nPos);

	DECLARE_MESSAGE_MAP()
};


class CPropertiesWnd : public CDockablePane
{
// Construction
public:
	void AdjustLayout() override;

// Attributes
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

	CNMRIDoc* theDoc = nullptr;

private:
	CMFCPropertyGridCtrl m_wndPropList;

// Implementation
	void InitPropList();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyChanged(__in WPARAM wparam, __in LPARAM lparam);

	DECLARE_MESSAGE_MAP()
};

