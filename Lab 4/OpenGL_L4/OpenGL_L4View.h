
// OpenGL_L4View.h : interface of the COpenGLL4View class
//

#pragma once

#include "CGLRenderer.h"

class COpenGLL4View : public CView
{
protected: // create from serialization only
	COpenGLL4View() noexcept;
	DECLARE_DYNCREATE(COpenGLL4View)

	// Attributes
public:
	COpenGLL4Doc* GetDocument() const;

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	void OnInitialUpdate();

	// Implementation
public:
	virtual ~COpenGLL4View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	CGLRenderer m_glRenderer;
	POINT mousePos = {0, 0};
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in OpenGL_L4View.cpp
inline COpenGLL4Doc* COpenGLL4View::GetDocument() const
   { return reinterpret_cast<COpenGLL4Doc*>(m_pDocument); }
#endif

