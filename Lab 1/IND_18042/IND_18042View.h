
// IND_18042View.h : interface of the CIND18042View class
//

#pragma once


class CIND18042View : public CView
{
protected: // create from serialization only
	CIND18042View() noexcept;
	DECLARE_DYNCREATE(CIND18042View)

// Attributes
public:
	CIND18042Doc* GetDocument() const;

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
	virtual ~CIND18042View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	// Za zadatak
private:
	bool isGridOn = false;
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawGrid(CDC* pDC);
	void DrawTrianglePattern(POINT p1, POINT p2, POINT p3, COLORREF fill, int pattern, COLORREF patternColor, CDC* pDC);
	void DrawTriangle(POINT p1, POINT p2, POINT p3, COLORREF fill, CDC* pDC);
	void DrawQuadrilateralPattern(POINT p1, POINT p2, POINT p3, POINT p4, COLORREF fill, int pattern, COLORREF patternColor, CDC* pDC);
	void DrawQuadrilateral(POINT p1, POINT p2, POINT p3, POINT p4, COLORREF fill, CDC* pDC);
	void DrawBackground(COLORREF backgroundColor, CDC* pDC);
public:
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_18042View.cpp
inline CIND18042Doc* CIND18042View::GetDocument() const
   { return reinterpret_cast<CIND18042Doc*>(m_pDocument); }
#endif

