
// L5View.cpp : implementation of the CL5View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "L5.h"
#endif

#include "L5Doc.h"
#include "L5View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CL5View

IMPLEMENT_DYNCREATE(CL5View, CView)

BEGIN_MESSAGE_MAP(CL5View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CL5View construction/destruction

CL5View::CL5View() noexcept
{
	// TODO: add construction code here
	m_glRenderer = CGLRenderer();
	mousePos = { 0, 0 };
}

CL5View::~CL5View()
{
}

BOOL CL5View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CL5View drawing

void CL5View::OnDraw(CDC* pDC)
{
	CL5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	m_glRenderer.DrawScene(pDC);
}


// CL5View printing

BOOL CL5View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CL5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CL5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CL5View diagnostics

#ifdef _DEBUG
void CL5View::AssertValid() const
{
	CView::AssertValid();
}

void CL5View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CL5Doc* CL5View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CL5Doc)));
	return (CL5Doc*)m_pDocument;
}
#endif //_DEBUG


// CL5View message handlers


int CL5View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);
	return 0;
}


BOOL CL5View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CL5View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
		this->m_glRenderer.viewAngle[1] += 2 * PI / 180 * m_glRenderer.upVector[1];
		Invalidate();
	}
	if (nChar == 'D') {
		this->m_glRenderer.viewAngle[1] -= 2 * PI / 180 * m_glRenderer.upVector[1];
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
	if (nChar == 'G') {
		this->m_glRenderer.showGrid = !this->m_glRenderer.showGrid;
		Invalidate();
	}
	if (nChar == 'H') {
		this->m_glRenderer.showAxis = !this->m_glRenderer.showAxis;
		Invalidate();
	}
	if (nChar == 'N') {
		this->m_glRenderer.showNormals = !this->m_glRenderer.showNormals;
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
		double r = this->m_glRenderer.viewR - 1.2;

		if (r > 1) {
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
	if (nChar == '1') {
		this->m_glRenderer.l1 = !this->m_glRenderer.l1;
		Invalidate();
	}
	if (nChar == '2') {
		this->m_glRenderer.l2 = !this->m_glRenderer.l2;
		Invalidate();
	}
	if (nChar == '3') {
		this->m_glRenderer.l3 = !this->m_glRenderer.l3;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CL5View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CL5View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL CL5View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	double r = this->m_glRenderer.viewR + zDelta * (-0.01);

	if (r > 1 && r < 50) {
		this->m_glRenderer.viewR = r;
		Invalidate();
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CL5View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON) {
		double moveX = point.x > mousePos.x ? 2 : -2;
		double moveY = point.y > mousePos.y ? 2 : -2;
		if (abs(point.x - mousePos.x) > abs(point.y - mousePos.y))
			moveY /= 10;
		else
			moveX /= 10;
		this->m_glRenderer.viewAngle[1] += moveX * PI / 180 * m_glRenderer.upVector[1];
		this->m_glRenderer.viewAngle[0] += moveY * PI / 180;
		mousePos = point;
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}