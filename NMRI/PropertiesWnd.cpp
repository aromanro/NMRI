
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "NMRI.h"

#include "NMRIDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// for CSliderProp and CPropSliderCtrl see: https://github.com/jhlee8804/MFC-Feature-Pack/tree/master/NewControls
// the code is adjusted to allow setting the range

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl

CPropSliderCtrl::CPropSliderCtrl(CSliderProp* pProp, COLORREF clrBack) {
	m_clrBack = clrBack;
	m_brBackground.CreateSolidBrush(m_clrBack);
	m_pProp = pProp;
}

CPropSliderCtrl::~CPropSliderCtrl() {
}

BEGIN_MESSAGE_MAP(CPropSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CPropSliderCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_HSCROLL_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl message handlers

HBRUSH CPropSliderCtrl::CtlColor(CDC* pDC, UINT /*nCtlColor*/) {
	pDC->SetBkColor(m_clrBack);
	return m_brBackground;
}

void CPropSliderCtrl::HScroll(UINT /*nSBCode*/, UINT /*nPos*/) {
	ASSERT_VALID(m_pProp);

	m_pProp->OnUpdateValue();
	m_pProp->Redraw();
}

////////////////////////////////////////////////////////////////////////////////
// CSliderProp class

CSliderProp::CSliderProp(const CString& strName, long nValue, long minVal, long maxVal, LPCTSTR lpszDescr, DWORD dwData) :
	CMFCPropertyGridProperty(strName, nValue, lpszDescr, dwData), m_minVal(minVal), m_maxVal(maxVal)
{
}

CWnd* CSliderProp::CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat) {
	CPropSliderCtrl* pWndSlider = new CPropSliderCtrl(this, m_pWndList->GetBkColor());

	rectEdit.left += rectEdit.Height() + 5;

	pWndSlider->Create(WS_VISIBLE | WS_CHILD, rectEdit, m_pWndList, AFX_PROPLIST_ID_INPLACE);
	pWndSlider->SetRange(m_minVal, m_maxVal);
	pWndSlider->SetPos(m_varValue.lVal);

	bDefaultFormat = TRUE;
	return pWndSlider;
}

BOOL CSliderProp::OnUpdateValue() {
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT_VALID(m_pWndList);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));

	long lCurrValue = m_varValue.lVal;

	CSliderCtrl* pSlider = (CSliderCtrl*)m_pWndInPlace;

	m_varValue = (long)pSlider->GetPos();

	if (lCurrValue != m_varValue.lVal) {
		m_pWndList->OnPropertyChanged(this);
	}

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
	: theDoc(nullptr)
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();


	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();


	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}


void CPropertiesWnd::InitPropList()
{
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Filter"));

	CMFCPropertyGridProperty *prop = new CMFCPropertyGridProperty(_T("Low Frequencies"), (_variant_t)false, _T("Removes the low frequency information"));

	prop->SetData(0);
	pGroup1->AddSubItem(prop);

	prop = new CMFCPropertyGridProperty(_T("High Frequencies"), (_variant_t)false, _T("Removes the high frequency information"));
	prop->SetData(1);
	pGroup1->AddSubItem(prop);

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("3D View"));

	prop = new CMFCPropertyGridProperty(_T("Color transfer function"), (_variant_t)true, _T("Color the 3D image with blue for low values, red for high"));
	prop->SetData(2);
	pGroup2->AddSubItem(prop);

	prop = new CMFCPropertyGridProperty(_T("Scalar opacity transfer function"), (_variant_t)true, _T("Make the small values more transparent than the big ones"));
	prop->SetData(3);
	pGroup2->AddSubItem(prop);

	prop = new CMFCPropertyGridProperty(_T("Gradient opacity transfer function"), (_variant_t)true, _T("Make the low gradient values more transparent"));
	prop->SetData(4);
	pGroup2->AddSubItem(prop);

	prop = new CSliderProp(_T("Opacity point"), 50, 0, 100, _T("Opacity point where the value is 1"));
	prop->SetData(5);
	pGroup2->AddSubItem(prop);

	prop = new CSliderProp(_T("Gradient point"), 50, 0, 100, _T("Gradient point where the value is 1"));
	prop->SetData(6);
	pGroup2->AddSubItem(prop);

	m_wndPropList.AddProperty(pGroup2);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
}

LRESULT CPropertiesWnd::OnPropertyChanged(__in WPARAM /*wparam*/, __in LPARAM lparam)
{
	if (!theDoc) return 0;
	
	const CMFCPropertyGridProperty *prop = (CMFCPropertyGridProperty *)lparam;


	 if (prop)
	 {
		 COleVariant v = prop->GetValue();

		 const auto opt = prop->GetData();
		 switch (opt)
		 {
			case 0:
				v.ChangeType(VT_BOOL);
				theDoc->theFile.filterLowFreqs = v.boolVal;
				break;
			case 1:
				v.ChangeType(VT_BOOL);
				theDoc->theFile.filterHighFreqs = v.boolVal;
				break;
			case 2:
				v.ChangeType(VT_BOOL);
				theDoc->colorFunction = v.boolVal;
				break;
			case 3:
				v.ChangeType(VT_BOOL);
				theDoc->opacityFunction = v.boolVal;
				break;
			case 4:
				v.ChangeType(VT_BOOL);
				theDoc->gradientFunction = v.boolVal;
				break;
			case 5:
				v.ChangeType(VT_INT);
				theDoc->opacityVal = v.intVal;
				break;
			case 6:
				v.ChangeType(VT_INT);
				theDoc->gradientVal = v.intVal;
				break;
		 }

		 if (opt <= 1)
			 theDoc->UpdateViews();
		 else
			 theDoc->Update3DOptions();
	 }

	 return 0;
}


