
// OpenGL_L4View.cpp : implementation of the COpenGLL4View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGL_L4.h"
#endif

#include "OpenGL_L4Doc.h"
#include "OpenGL_L4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLL4View

IMPLEMENT_DYNCREATE(COpenGLL4View, CView)

BEGIN_MESSAGE_MAP(COpenGLL4View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// COpenGLL4View construction/destruction

COpenGLL4View::COpenGLL4View() noexcept
{
	m_glRenderer = CGLRenderer();

}

COpenGLL4View::~COpenGLL4View()
{
}

BOOL COpenGLL4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COpenGLL4View drawing

void COpenGLL4View::OnDraw(CDC* pDC)
{
	COpenGLL4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_glRenderer.DrawScene(pDC);
}


// COpenGLL4View printing

BOOL COpenGLL4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGLL4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGLL4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void COpenGLL4View::OnInitialUpdate()
{
	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);

}


// COpenGLL4View diagnostics

#ifdef _DEBUG
void COpenGLL4View::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLL4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLL4Doc* COpenGLL4View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLL4Doc)));
	return (COpenGLL4Doc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLL4View message handlers


int COpenGLL4View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);
	return 0;
}


BOOL COpenGLL4View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void COpenGLL4View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'Q') {
		this->m_glRenderer.alpha -= 2;
		Invalidate();
	}
	if (nChar == 'E') {
		this->m_glRenderer.alpha += 2;
		Invalidate();
	}
	if (nChar == 'A') {
		this->m_glRenderer.viewAngle[1] += 2 * PI / 180;
		Invalidate();
	}
	if (nChar == 'D') {
		this->m_glRenderer.viewAngle[1] -= 2 * PI / 180;
		Invalidate();
	}
	if (nChar == 'W') {
		this->m_glRenderer.viewAngle[0] += 2 * PI / 180;
		Invalidate();
	}
	if (nChar == 'S') {
		this->m_glRenderer.viewAngle[0] -= 2 * PI / 180;
		Invalidate();
	}
	if (nChar == VK_UP) {
		this->m_glRenderer.lookAtHeight += 0.5;
		Invalidate();
	}
	if (nChar == VK_DOWN) {
		this->m_glRenderer.lookAtHeight -= 0.5;
		Invalidate();
	}
	if (nChar == 107) {
		double r = this->m_glRenderer.viewR -1.2;

		if (r > 5) {
			this->m_glRenderer.viewR = r;
			Invalidate();
		}
	}
	if (nChar == 109) {
		double r = this->m_glRenderer.viewR + 1.2;

		if (r < 50) {
			this->m_glRenderer.viewR = r;
			Invalidate();
		}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void COpenGLL4View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void COpenGLL4View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL COpenGLL4View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	double r = this->m_glRenderer.viewR + zDelta * (-0.01);

	if (r > 5 && r < 50) {
		this->m_glRenderer.viewR = r;
		Invalidate();
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void COpenGLL4View::OnMove(int x, int y)
{


	CView::OnMove(x, y);
}


void COpenGLL4View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON) {
		double moveX = point.x > mousePos.x ? 1 : -1;
		double moveY = point.y > mousePos.y ? 1 : -1;
		if (abs(point.x - mousePos.x) > abs(point.y - mousePos.y))
			moveY /= 10;
		else
			moveX /= 10;
		this->m_glRenderer.viewAngle[1] += moveX * PI / 180;
		this->m_glRenderer.viewAngle[0] += moveY * PI / 180;
		mousePos = point;
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}
