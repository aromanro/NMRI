
// NMRI.h : main header file for the NMRI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CNMRIApp:
// See NMRI.cpp for the implementation of this class
//

class CNMRIApp : public CWinAppEx
{
public:
	CNMRIApp();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

private:
	// Overrides
	BOOL InitInstance() override;
	void PreLoadState() override;
	void LoadCustomState() override;
	void SaveCustomState() override;

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNMRIApp theApp;
