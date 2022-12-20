
// L5.h : main header file for the L5 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CL5App:
// See L5.cpp for the implementation of this class
//

class CL5App : public CWinApp
{
public:
	CL5App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CL5App theApp;
