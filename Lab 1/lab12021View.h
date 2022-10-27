
// lab12021View.h : interface of the Clab12021View class
//
#pragma once


class Clab12021View : public CView
{
protected: // create from serialization only
	Clab12021View() noexcept;
	DECLARE_DYNCREATE(Clab12021View)

// Attributes
public:
	Clab12021Doc* GetDocument() const;

// Operations
public:
	bool on = false;

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
	virtual ~Clab12021View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in lab12021View.cpp
inline Clab12021Doc* Clab12021View::GetDocument() const
   { return reinterpret_cast<Clab12021Doc*>(m_pDocument); }
#endif

