
// L3_2View.cpp : implementation of the CL32View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "L3_2.h"
#endif

#include "L3_2Doc.h"
#include "L3_2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CL32View

IMPLEMENT_DYNCREATE(CL32View, CView)

BEGIN_MESSAGE_MAP(CL32View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CL32View construction/destruction

CL32View::CL32View() noexcept
{
	// TODO: add construction code here
	LoadPuzzlePieces();
}

CL32View::~CL32View()
{
	for (int i = 0; i < MAX_PIECES; i++) {
		delete puzzlePieces[i];
	}
}

BOOL CL32View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/// lab

void CL32View::LoadPuzzlePieces()
{
	char s[] = "1.bmp";
	for (int i = 0; i < MAX_PIECES; i++) {
		s[0] = '0' + (i + 1);
		puzzlePieces[i] = new DImage();
		puzzlePieces[i]->Load(CString(s));
	}
}
CBitmap* CL32View::GetBitmapPuzzlePiece(CDC* pDC, int i) {
	CDC* temp = new CDC();
	temp->CreateCompatibleDC(pDC);
	CBitmap* ret = new CBitmap();
	ret->CreateCompatibleBitmap(pDC, puzzlePieces[i]->Width(), puzzlePieces[i]->Height());

	CBitmap* old = temp->SelectObject(ret);
	CRect src(0, 0, puzzlePieces[i]->Width(), puzzlePieces[i]->Height());
	CRect dest(0, 0, puzzlePieces[i]->Width(), puzzlePieces[i]->Height());
	puzzlePieces[i]->Draw(temp, src, dest);

	temp->SelectObject(old);
	delete temp;
	return ret;
}

void CL32View::DrawTransparent(CDC* pDC, DImage* img)
{
	CRect rect(0, 0, img->Width(), img->Height());
	int w = rect.Width();
	int h = rect.Height();

	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* src = new CBitmap();
	CBitmap* dst = new CBitmap();
	src->CreateCompatibleBitmap(pDC, w, h);
	dst->CreateBitmap(w, h, 1, 1, NULL);

	CBitmap* oldSrc = srcDC->SelectObject(src);
	CBitmap* oldDst = dstDC->SelectObject(dst);

	img->Draw(srcDC, rect, rect);
	srcDC->SetBkColor(srcDC->GetPixel({ 0,0 }));
	dstDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCCOPY);

	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);

	//Da centriramo sliku
	Translate(pDC, -w / 2, -h / 2);

	pDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCPAINT);

	Translate(pDC, w / 2, h / 2);

	srcDC->SelectObject(oldSrc);
	dstDC->SelectObject(oldDst);


	delete srcDC;
	delete dstDC;
	delete src;
	delete dst;
}

void CL32View::DrawTransparent(CDC* pDC, DImage* img, bool isBlue) {
	
	int w = img->Width();
	int h = img->Height();

	CBitmap srcBitmap;
	srcBitmap.CreateCompatibleBitmap(pDC, w, h);

	CBitmap maskBitmap;
	maskBitmap.CreateBitmap(w, h, 1, 1, nullptr);

	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);

	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(dstDC);

	CBitmap* oldSrcBitmap = srcDC->SelectObject(&srcBitmap);
	CBitmap* oldDstBitmap = dstDC->SelectObject(&maskBitmap);

	img->Draw(srcDC, CRect(0, 0, w, h), CRect(0, 0, w, h));
	if (isBlue)
		makeBlue(&srcBitmap);
	else
		makeGray(&srcBitmap);
	COLORREF trColor = srcDC->GetPixel(0, 0);
	COLORREF oldBgColor = srcDC->SetBkColor(trColor);

	dstDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCCOPY);

	COLORREF oldTextColorSrc = srcDC->SetTextColor(RGB(255, 255, 255));
	COLORREF oldBackgroundColorSrc = srcDC->SetBkColor(RGB(0,0,0));

	srcDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);
	
	Translate(pDC, -w / 2, -h / 2);

	pDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);

	pDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCPAINT);
	
	Translate(pDC, w / 2, h / 2);

	srcDC->SetTextColor(oldTextColorSrc);
	srcDC->SetBkColor(oldBgColor);

	srcDC->SelectObject(oldSrcBitmap);
	dstDC->SelectObject(oldDstBitmap);

	srcDC->DeleteDC();
	delete srcDC;
	
	dstDC->DeleteDC();
	delete dstDC;
}
void CL32View::makeGray(CBitmap* bitmap) {
	BITMAP b;
	bitmap->GetBitmap(&b);

	BYTE* bits = new BYTE[b.bmWidthBytes * b.bmHeight];
	bitmap->GetBitmapBits(b.bmWidthBytes * b.bmHeight, bits);
	COLORREF trColor = RGB(bits[2], bits[1], bits[0]);

	for (int i = 0; i < b.bmWidthBytes * b.bmHeight; i += 4)
	{
		if (RGB(bits[i+2], bits[i+1], bits[i]) == trColor) continue;
		BYTE gr = min(255, (bits[i] + bits[i + 1] + bits[i + 2]) / 3 + 64);
		bits[i] = bits[i + 1] = bits[i + 2] = gr;
	}

	bitmap->SetBitmapBits(b.bmWidthBytes * b.bmHeight, bits);

	if (bits)
		delete[] bits;

	bits = nullptr;
}
void CL32View::makeBlue(CBitmap* bitmap) {
	BITMAP b;
	bitmap->GetBitmap(&b);

	BYTE* bits = new BYTE[b.bmWidthBytes * b.bmHeight];
	bitmap->GetBitmapBits(b.bmWidthBytes * b.bmHeight, bits);
	COLORREF trColor = RGB(bits[2], bits[1], bits[0]);

	for (int i = 0; i < b.bmWidthBytes * b.bmHeight; i += 4) {
		if (RGB(bits[i + 2], bits[i + 1], bits[i]) == trColor) continue;
		BYTE gr = min(255, (bits[i] + bits[i + 1] + bits[i + 2]) / 3 + 64);
		bits[i] = gr;
		bits[i + 1] = bits[i + 2] = 0;
	}

	bitmap->SetBitmapBits(b.bmWidthBytes * b.bmHeight, bits);

	if (bits)
		delete[] bits;

	bits = nullptr;
}

void CL32View::DrawGrid(CDC* pDC)
{
	if (!this->isGridOn)
		return;
	CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
	CPen* old = pDC->SelectObject(&pen);
	for (int i = 0; i < 20; i++) {
		pDC->MoveTo(i * 25, 0);
		pDC->LineTo(i * 25, 500);
		pDC->MoveTo(0, i * 25);
		pDC->LineTo(500, i * 25);
	}
	pDC->SelectObject(old);
}


void CL32View::DrawBackground(COLORREF backgroundColor, CDC* pDC)
{
	CBrush brush(backgroundColor);
	CRect client;
	GetClientRect(&client);

	pDC->FillRect(client, &brush);
}

void CL32View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CL32View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CL32View::Mirror(CDC* pDC, bool mx, bool my, bool rightMyltiply)
{
	Scale(pDC, mx ? -1 : 1, my ? -1 : 1, rightMyltiply);
}

void CL32View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float sindeg = sin(angle);
	float cosdeg = cos(angle);
	XFORM form = { cosdeg, sindeg, -sindeg, cosdeg, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CL32View::RotateFrom(CDC* pDC, int xRot, int yRot, float angle, bool rightMultiply)
{
	Translate(pDC, xRot, yRot, rightMultiply);
	Rotate(pDC, angle, rightMultiply);
	Translate(pDC, -xRot, -yRot, rightMultiply);
}

void CL32View::DrawMem(CDC* pDC)
{
	DrawBackground(RGB(255, 255, 255), pDC);
	DrawGrid(pDC);
	
	CRect rect;
	GetClientRect(&rect);

	XFORM tr;
	pDC->GetWorldTransform(&tr);

	Translate(pDC, 100 + 6, 100+6);
	Rotate(pDC, PI * 6/7 + RAD(1));
	Mirror(pDC, true, false);
	DrawTransparent(pDC, puzzlePieces[0], false);
	pDC->SetWorldTransform(&tr);

	Translate(pDC, 25 * 10 -6, 25 * 10);
	Mirror(pDC, true, false);
	Rotate(pDC, PI/3 - RAD(2));
	DrawTransparent(pDC, puzzlePieces[1], false);
	pDC->SetWorldTransform(&tr);

	Translate(pDC, 25 * 16 - 6, 25 * 10 + 3);
	Mirror(pDC, true, false);
	Rotate(pDC, PI * 2.0 / 6 + RAD(12));
	DrawTransparent(pDC, puzzlePieces[2], false);
	pDC->SetWorldTransform(&tr);

	Translate(pDC, 25 * 4 - 6, 25 * 16 - 6);
	Mirror(pDC, true, false);
	Rotate(pDC, RAD(19));
	DrawTransparent(pDC, puzzlePieces[3], false);
	pDC->SetWorldTransform(&tr);

	Translate(pDC, 25 * 16 + 6, 25*4 - 4);
	Mirror(pDC, true, false);
	Rotate(pDC, RAD(14) - PI/2);
	DrawTransparent(pDC, puzzlePieces[4], false);
	pDC->SetWorldTransform(&tr);

	Translate(pDC, 25 * 10 - 6, 25 * 16 + 5);
	Mirror(pDC, true, false);
	Rotate(pDC, RAD(23) + PI/2);
	DrawTransparent(pDC, puzzlePieces[5], true);
	pDC->SetWorldTransform(&tr);


	Translate(pDC, 25 * 10 + 8, 25 * 4 + 3);
	Mirror(pDC, true, false);
	Rotate(pDC, -RAD(35) - PI/2);
	DrawTransparent(pDC, puzzlePieces[6], false);
	pDC->SetWorldTransform(&tr);

	Translate(pDC, 25 * 16 +2 , 25 * 16 + 8);
	Mirror(pDC, true, false);
	Rotate(pDC, -RAD(19) + PI);
	DrawTransparent(pDC, puzzlePieces[7], false);
	pDC->SetWorldTransform(&tr);

	Translate(pDC, 25 * 4 - 6, 25 * 10 + 6);
	Mirror(pDC, true, false);
	Rotate(pDC, RAD(16.5) + PI/2);
	DrawTransparent(pDC, puzzlePieces[8], false);
	pDC->SetWorldTransform(&tr);
}


void CL32View::OnDraw(CDC* pDC)
{
	CL32Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	int orgmode = pDC->SetGraphicsMode(GM_ADVANCED);

	CDC* mem = new CDC();
	mem->CreateCompatibleDC(pDC);

	int gmode = mem->SetGraphicsMode(GM_ADVANCED);

	CBitmap memb;
	memb.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* old = mem->SelectObject(&memb);

	DrawMem(mem);

	Translate(pDC, 250, 250);
	Rotate(pDC, rotation * PI / 2);
	Mirror(pDC, mx, my);
	Translate(pDC, -250, -250);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), mem, 0, 0, SRCCOPY);

	pDC->SetGraphicsMode(orgmode);

	mem->SetGraphicsMode(gmode);
	mem->SelectObject(old);
	mem->DeleteDC();
	delete mem;
}




// CL32View printing

BOOL CL32View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CL32View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CL32View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CL32View diagnostics

#ifdef _DEBUG
void CL32View::AssertValid() const
{
	CView::AssertValid();
}

void CL32View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CL32Doc* CL32View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CL32Doc)));
	return (CL32Doc*)m_pDocument;
}
#endif //_DEBUG


// CL32View message handlers


void CL32View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G') {
		this->isGridOn = !this->isGridOn;
		Invalidate();
	}
	if (nChar == 'Y') {
		this->mx = !this->mx;
		Invalidate();
	}
	if (nChar == 'X') {
		this->my = !this->my;
		Invalidate();
	}
	if (nChar == 'R') {
		this->rotation = (this->rotation + 1) % 4;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
