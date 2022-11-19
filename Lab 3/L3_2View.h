
// L3_2View.h : interface of the CL32View class
//

#pragma once

#include "DImage.h"
#define MAX_PIECES 9

#define PI 3.14159

#define RAD(x) x * 2.0 * PI / 360
#define DEG(x) x* 360.0 / (2 * PI)

class CL32View : public CView
{
protected: // create from serialization only
	CL32View() noexcept;
	DECLARE_DYNCREATE(CL32View)

// Attributes
public:
	CL32Doc* GetDocument() const;

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

// Implementation
public:
	virtual ~CL32View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	//za lab
	int rotation = 0; // Postavi broj na koju stranu zelis
	bool mx = false;
	bool my = false;
	bool isGridOn = true, toggle = false;
	DImage* puzzlePieces[MAX_PIECES];
	void LoadPuzzlePieces();
	CBitmap* GetBitmapPuzzlePiece(CDC* pDC, int i);
	void DrawTransparent(CDC* pDC, DImage* img);
	void DrawTransparent(CDC* pDC, DImage* img, bool isBlue);
	void makeGray(CBitmap* bitmap);
	void makeBlue(CBitmap* bitmap);
	void DrawGrid(CDC* pDC);
	void DrawBackground(COLORREF backgroundColor, CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMyltiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void RotateFrom(CDC* pDC, int xRot, int yRot, float angle, bool rightMultiply = false);
	void DrawMem(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in L3_2View.cpp
inline CL32Doc* CL32View::GetDocument() const
   { return reinterpret_cast<CL32Doc*>(m_pDocument); }
#endif

