
// IND_18042.h : main header file for the IND_18042 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND18042App:
// See IND_18042.cpp for the implementation of this class
//

class CIND18042App : public CWinApp
{
public:
	CIND18042App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND18042App theApp;
