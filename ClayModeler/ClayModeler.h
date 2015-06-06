
// ClayModeler.h : main header file for the ClayModeler application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CClayModelerApp:
// See ClayModeler.cpp for the implementation of this class
//

class CClayModelerApp : public CWinAppEx
{
public:
	CClayModelerApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CClayModelerApp theApp;
