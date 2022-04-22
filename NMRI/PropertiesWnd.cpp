
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

		 v.ChangeType(VT_BOOL);

		 const auto opt = prop->GetData();
		 switch (opt)
		 {
			case 0:
				theDoc->theFile.filterLowFreqs = v.boolVal;
				break;
			case 1:
				theDoc->theFile.filterHighFreqs = v.boolVal;
				break;
			case 2:
				theDoc->colorFunction = v.boolVal;
				break;
			case 3:
				theDoc->opacityFunction = v.boolVal;
				break;
			case 4:
				theDoc->gradientFunction = v.boolVal;
				break;
		 }

		 if (opt <= 1)
			 theDoc->UpdateViews();
		 else
			 theDoc->Update3DOptions();
	 }

	 return 0;
}


