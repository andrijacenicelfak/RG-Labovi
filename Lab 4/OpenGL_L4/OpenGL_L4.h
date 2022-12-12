
// OpenGL_L4.h : main header file for the OpenGL_L4 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COpenGLL4App:
// See OpenGL_L4.cpp for the implementation of this class
//

class COpenGLL4App : public CWinApp
{
public:
	COpenGLL4App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGLL4App theApp;
