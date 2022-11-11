
// IND_18042View.cpp : implementation of the CIND18042View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18042.h"
#endif

#include "IND_18042Doc.h"
#include "IND_18042View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND18042View

IMPLEMENT_DYNCREATE(CIND18042View, CView)

BEGIN_MESSAGE_MAP(CIND18042View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND18042View construction/destruction

CIND18042View::CIND18042View() noexcept
{
	for (int i = 0; i < MAX_SEGMENTS; i++)
		deg[i] = 0.0f;
	CString ls("cactus_part_light.emf");
	CString ds("cactus_part.emf");
	light = GetEnhMetaFile(ls);
	dark = GetEnhMetaFile(ds);
	deg[2] = 1;
	deg[3] = -1;
	deg[4] = 1;
	deg[5] = -0.6;
	deg[7] = -0.9;
	deg[8] = 0.5;
	deg[9] = 0.5;
	deg[10] = -0.5;
}

CIND18042View::~CIND18042View()
{
	DeleteEnhMetaFile(light);
	DeleteEnhMetaFile(dark);
}

BOOL CIND18042View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

//LAB
void CIND18042View::DrawBackground(COLORREF backgroundColor, CDC* pDC) {
	CBrush brush(backgroundColor);
	pDC->FillRect(CRect(0, 0, 500, 500), &brush);

}
void CIND18042View::DrawGrid(CDC* pDC)
{
	if (!this->isGridOn)
		return;
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* old = pDC->SelectObject(&pen);
	for (int i = 0; i < 20; i++) {
		pDC->MoveTo(i * 25, 0);
		pDC->LineTo(i * 25, 500);
		pDC->MoveTo(0, i * 25);
		pDC->LineTo(500, i * 25);
	}
	pDC->SelectObject(old);
}
void CIND18042View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply) {
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CIND18042View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply) {
	XFORM form = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CIND18042View::Rotate(CDC* pDC, float angle, bool rightMultiply) {
	float sindeg = sin(angle);
	float cosdeg = cos(angle);
	XFORM form = { cosdeg, sindeg, -sindeg, cosdeg, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CIND18042View::RotateFrom(CDC* pDC, int xRot, int yRot, float angle, bool rightMultiply) {
	Translate(pDC, xRot, yRot, rightMultiply);
	Rotate(pDC, angle, rightMultiply);
}
void CIND18042View::DrawPot(CDC* pDC) {
	POINT bottom[] = { {-25 * 2, -25 * 2},{-(25 + 12), 0}, {25 + 12, 0}, {25 * 2, -25 * 2} };
	POINT top[] = { {-25 * 2 - 12, -25 * 2-20}, {-25 * 2 - 12, -25 * 2},{25 * 2 + 12, -25 * 2}, {25 * 2 + 12, -25 * 2 - 20} };
	CBrush brush(RGB(200, 100, 0));
	CBrush* old = pDC->SelectObject(&brush);
	pDC->Polygon(bottom, 4);
	pDC->Polygon(top, 4);
	pDC->SelectObject(old);
}
void CIND18042View::drawBranch(CDC* pdc,int width,int height, bool isLight) {
	CBrush brush(RGB(40, 220, 40));
	CBrush* old = pdc->SelectObject(&brush);
	CRect grana(-width / 2, -height, width / 2, 0);
	PlayEnhMetaFile(pdc->m_hDC, isLight ? light : dark, grana);
	CRect bounds(-RADIUS, -RADIUS, RADIUS, RADIUS);
	pdc->Ellipse(bounds);
	
	pdc->SelectObject(old);
}
void CIND18042View::DrawTextHere(CDC* pDC, const char* tekst, int x, int y, int size, float angle, COLORREF color) {
	COLORREF prev = pDC->SetTextColor(color);
	CString name("Arial");
	LOGFONT l;
	l.lfCharSet = ANSI_CHARSET;
	l.lfHeight = size;
	l.lfWeight = FW_BOLD;
	l.lfOrientation = 0;
	l.lfEscapement = 0;
	wcscpy_s(l.lfFaceName, 32, L"Times New Roman");

	CFont f;
	f.CreateFont(size, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, name);

	CFont* old = pDC->SelectObject(&f);

	int o = pDC->SetBkMode(TRANSPARENT);

	Translate(pDC, x, y);
	Rotate(pDC, angle);
		pDC->TextOut(0, 0, CString(tekst));
	Rotate(pDC, -angle);
	Translate(pDC, -x, -y);

	pDC->SetBkMode(o);

	pDC->SelectObject(old);
	pDC->SetTextColor(prev);
}
void CIND18042View::DrawFigure(CDC* pDC) {
	//POCETAK
	Rotate(pDC, deg[0]);
	drawBranch(pDC, 60, 80, true);
	//dece ovog idu nivo vise zbog preglednosti
	{
		Translate(pDC, 0, -80);

		Rotate(pDC, deg[1]);
		drawBranch(pDC, 20, 80);
		Rotate(pDC, -deg[1]);

		Rotate(pDC, deg[2]);
		drawBranch(pDC, 20, 80);
		{
			Translate(pDC, 0, -80);

			Rotate(pDC, deg[7]);
			drawBranch(pDC, 40, 80, true);
			Rotate(pDC, -deg[7]);

			Rotate(pDC, deg[8]);
			drawBranch(pDC, 40, 80);
			{
				Translate(pDC, 0, -80);

				Rotate(pDC, deg[9]);
				drawBranch(pDC, 40, 80);
				Rotate(pDC, -deg[9]);

				Rotate(pDC, deg[10]);
				drawBranch(pDC, 40, 80);
				Rotate(pDC, -deg[10]);

				Translate(pDC, 0, 80);
			}
			Rotate(pDC, -deg[8]);

			Translate(pDC, 0, 80);
		}
		Rotate(pDC, -deg[2]);

		Rotate(pDC, deg[3]);
		drawBranch(pDC, 20, 80);
		{
			Translate(pDC, 0, -80);

			Rotate(pDC, deg[4]);
			drawBranch(pDC, 40, 80);
			{
				Translate(pDC, 0, -80);

				Rotate(pDC, deg[6]);
				drawBranch(pDC, 50, 80);
				Rotate(pDC, -deg[6]);

				Translate(pDC, 0, 80);
			}
			Rotate(pDC, -deg[4]);

			Rotate(pDC, deg[5]);
			drawBranch(pDC, 40, 80);
			Rotate(pDC, -deg[5]);

			Translate(pDC, 0, 80);
		}
		Rotate(pDC, -deg[3]);

		Translate(pDC, 0, 80);
	}
	Rotate(pDC, -deg[0]);
}

void CIND18042View::OnDraw(CDC* pDC)
{
	CIND18042Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	int mode = pDC->SetGraphicsMode(GM_ADVANCED);
	DrawBackground(RGB(180, 180, 255), pDC);
	Translate(pDC, 250, 500);
	Translate(pDC, 0, -25 * 2 - 20);
	DrawFigure(pDC);
	Translate(pDC, 0, 25 * 2 + 20);
	DrawPot(pDC);
	Translate(pDC, -250, -500);
	DrawTextHere(pDC, "18042 Andrija Cenic", 462, 22, 40, 3.141 / 2, RGB(0, 0, 0));
	DrawTextHere(pDC, "18042 Andrija Cenic", 460, 20, 40, 3.141 / 2, RGB(240,240, 30));
	DrawGrid(pDC);

	pDC->SetGraphicsMode(mode);
}


// CIND18042View printing

BOOL CIND18042View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18042View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18042View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND18042View diagnostics

#ifdef _DEBUG
void CIND18042View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18042View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18042Doc* CIND18042View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18042Doc)));
	return (CIND18042Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18042View message handlers


void CIND18042View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == ' ') {
		this->isGridOn = !this->isGridOn;
		Invalidate();
	}
	if (nChar == 'Z') toggle = !toggle;
	if (nChar >= 'A' && nChar < 'A' + 16) {
		int index = nChar - 'A';
		deg[index] += 0.1 * ( toggle ? -1 : 1);
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
