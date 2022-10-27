// lab12021View.cpp : implementation of the Clab12021View class
//
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "lab12021.h"
#endif

#include "lab12021Doc.h"
#include "lab12021View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>

using namespace std;

// Clab12021View

IMPLEMENT_DYNCREATE(Clab12021View, CView)

BEGIN_MESSAGE_MAP(Clab12021View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Clab12021View construction/destruction

Clab12021View::Clab12021View() noexcept
{
	// TODO: add construction code here

}

Clab12021View::~Clab12021View()
{
}

BOOL Clab12021View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {
	CBrush* old = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);
	float rot = 2 * 3.1415926535 / n;
	POINT* points = new POINT[n];
	
	for (int i = 0; i < n; i++) {
		points[i].x = r * cos(rotAngle) + cx;
		points[i].y = r * sin(rotAngle) + cy;
		rotAngle += rot;
	}
	pDC->Polygon(points, n);
	pDC->SelectObject(old);
}
void DrawGrid(CDC* pDC) {
	for (int i = 0; i < 20; i++) {
		pDC->MoveTo(i * 25, 0);
		pDC->LineTo(i * 25, 500);
		pDC->MoveTo(0, i * 25);
		pDC->LineTo(500, i * 25);
	}
}

void DrawTrianglePattern(POINT p1, POINT p2, POINT p3, COLORREF fill, int pattern, COLORREF patternColor, CDC* pDC) {
	COLORREF bck = pDC->GetBkColor();
	pDC->SetBkColor(fill);
	CBrush brush(pattern, patternColor);
	CBrush* old = pDC->SelectObject(&brush);

	POINT* points = new POINT[3];
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;

	pDC->Polygon(points, 3);

	pDC->SelectObject(old);
	pDC->SetBkColor(bck);
}
void DrawTriangle(POINT p1, POINT p2, POINT p3, COLORREF fill, CDC* pDC) {
	DrawTrianglePattern(p1, p2, p3, fill, 0, fill, pDC);
}
void DrawQuadrilateralPattern(POINT p1, POINT p2, POINT p3, POINT p4, COLORREF fill, int pattern, COLORREF patternColor, CDC* pDC) {
	COLORREF bck = pDC->GetBkColor();
	pDC->SetBkColor(fill);
	CBrush brush(pattern, patternColor);
	CBrush* old = pDC->SelectObject(&brush);

	POINT* points = new POINT[4];
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;

	pDC->Polygon(points, 4);

	pDC->SelectObject(old);
	pDC->SetBkColor(bck);
}
void DrawQuadrilateral(POINT p1, POINT p2, POINT p3, POINT p4, COLORREF fill, CDC* pDC) {
	DrawQuadrilateralPattern(p1, p2, p3, p4, fill, 0, fill, pDC);
}

void Clab12021View::OnDraw(CDC* pDC)
{
	Clab12021Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//Background
	pDC->SetBkColor(RGB(155, 155, 155));
	CRect rect(0,0,500,500);
	CBrush* bk = new CBrush(RGB(200, 200, 200));
	pDC->FillRect(rect, bk);

	//Zelene olovka
	CPen* pen = new CPen(PS_SOLID, 5, RGB(120, 240, 0));
	CPen* old = pDC->SelectObject(pen);

	//Background iregular shapes
	//narandzasti trougao
	DrawTriangle({ 25,25 }, { 13 * 25, 13 * 25 }, { 13 * 25, 25 }, RGB(232, 159, 62), pDC);


	//israfirani trougao
	DrawTrianglePattern({ 25,25 }, { 25, 7 * 25 }, { 7 * 25, 7 * 25 }, RGB(255, 255, 255), HS_FDIAGONAL, RGB(0, 0, 0), pDC);

	//sivi trougao levi
	DrawTriangle({ 25, 25 * 7 }, { 25 * 7, 25 * 7 }, { 25 * 7, 25 *13}, RGB(220, 220, 220), pDC);

	//nepravilni zeleni 4-ugao
	DrawQuadrilateral({ 25, 25 * 7 }, { 25 * 7, 25 * 13 }, { 25 * 7, 25 * 19 }, { 25, 25 * 13 }, RGB(20, 200, 20), pDC);

	//Zuti trougao
	DrawTriangle({ 25, 25 * 13 }, { 25 * 7, 25 * 19 }, { 25, 25 * 19 }, RGB(240, 240, 0), pDC);

	//Roze trougao
	DrawTriangle({ 25 * 7, 25 * 19 }, { 25 * 13, 25 * 13 }, { 25 * 7, 25 * 7 }, RGB(255, 150, 150), pDC);

	//Crven trougao
	DrawTriangle({ 25 * 7, 25 * 19 }, { 25 * 19, 25 * 19 }, { 25 * 19, 25 * 7 }, RGB(255, 0, 0), pDC);

	//sivi desni
	DrawTriangle({ 25 * 19, 25 * 7 }, { 25 * 13, 25 * 7 }, { 25 * 13, 25 * 13 }, RGB(230, 230, 230), pDC);

	//Ljubicasti kvadrat
	DrawQuadrilateral({ 25 * 13, 25 }, { 25 * 19, 25 }, { 25 * 19, 25 * 7 }, { 25 * 13 , 25 * 7 }, RGB(180, 0, 180), pDC);

	//Regular polygons
	CPen regPen(PS_SOLID, 3, RGB(120, 240, 0));
	pDC->SelectObject(&regPen);

	//U narandzasti trougao
	DrawRegularPolygon(pDC, 25 * 10 - 12, 25 * 5 - 12, 45, 5, 0);
	//U srafirani beli
	DrawRegularPolygon(pDC, 62, 25 * 5 + 12, 20, 4, 0);
	//U zuti
	DrawRegularPolygon(pDC, 70, 25 * 17, 25, 8, 0);
	// u roze
	DrawRegularPolygon(pDC, 25 * 9 + 12, 25 * 13, 30, 7, 0);
	//U crveni
	DrawRegularPolygon(pDC, 25 * 15 + 12, 25 * 15 + 12, 40, 6, 0);

	//Draw grid
	CPen* gridPen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
	pDC->SelectObject(gridPen);
	if(on)
		DrawGrid(pDC);

	pDC->SelectObject(old);
	delete gridPen;
	delete pen;
	delete bk;
}


// Clab12021View printing

BOOL Clab12021View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Clab12021View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Clab12021View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Clab12021View diagnostics

#ifdef _DEBUG
void Clab12021View::AssertValid() const
{
	CView::AssertValid();
}

void Clab12021View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab12021Doc* Clab12021View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab12021Doc)));
	return (Clab12021Doc*)m_pDocument;
}
#endif //_DEBUG


// Clab12021View message handlers


void Clab12021View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G') {
		on = !on;
		InvalidateRect(NULL);
		UpdateWindow();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
