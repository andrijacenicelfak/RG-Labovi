
// IND_18042View.h : interface of the CIND18042View class
//

#pragma once
#define MAX_SEGMENTS 16
#define RADIUS 12
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
private:
	//Za LAB
	float deg[MAX_SEGMENTS];
	HENHMETAFILE dark, light;
	bool isGridOn = false, toggle = false;
	void DrawGrid(CDC* pDC);
	void DrawPot(CDC* pDC);
	void DrawBackground(COLORREF backgroundColor, CDC* pDC);
	void DrawFigure(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply= false);
	void RotateFrom(CDC* pDC, int xRot, int yRot, float angle, bool rightMultiply = false);
	void drawBranch(CDC* pDC, int width, int height, bool isLight = false);
	void DrawTextHere(CDC* pDC, const char* tekst, int x, int y, int size, float angle, COLORREF color);

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_18042View.cpp
inline CIND18042Doc* CIND18042View::GetDocument() const
   { return reinterpret_cast<CIND18042Doc*>(m_pDocument); }
#endif

