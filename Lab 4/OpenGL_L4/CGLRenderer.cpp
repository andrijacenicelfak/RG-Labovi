#include "pch.h"
#include "CGLRenderer.h"

void CGLRenderer::CalculateViewPosition()
{
	viewPosition[0] = viewR * cos(viewAngle[0]) * cos(viewAngle[1]);
	viewPosition[1] = viewR * sin(viewAngle[0]);
	viewPosition[2] = viewR * cos(viewAngle[0]) * sin(viewAngle[1]);


	lookingAt[0] = 0;
	lookingAt[1] = lookAtHeight;
	lookingAt[2] = 0;
}

CGLRenderer::CGLRenderer()
{
	this->m_hrc = NULL;
	viewR = 20;
	viewAngle[0] = PI * 30 / 180;
	viewAngle[1] = PI * 45 / 180;
	
	this->CalculateViewPosition();

	upVector[0] = 0;
	upVector[1] = 1;
	upVector[2] = 0;

}

CGLRenderer::~CGLRenderer()
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0)
		return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (bResult == FALSE)
		return false;

	this->m_hrc = wglCreateContext(pDC->m_hDC);

	if (this->m_hrc == NULL)
		return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, this->m_hrc);
	// -------------------------------------------

	glClearColor(0, .8, .0, .0);

	// -------------------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, this->m_hrc);
	// -------------------------------------------
	wglMakeCurrent(NULL, NULL);

	if (this->m_hrc != NULL)
	{
		wglDeleteContext(this->m_hrc);
		this->m_hrc = NULL;
	}
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, this->m_hrc);
	// -------------------------------------------

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);

	// -------------------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	CalculateViewPosition();
	wglMakeCurrent(pDC->m_hDC, this->m_hrc);
	// -------------------------------------------
	glClearColor(.7, .9, .7, .0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	gluLookAt(this->viewPosition[0], this->viewPosition[1], this->viewPosition[2],this->lookingAt[0], this->lookingAt[1], this->lookingAt[2],this->upVector[0], this->upVector[1], this->upVector[2]);

	if (this->showAxis)
		this->DrawAxis(2);

	//DrawCylinder(3, 2, 2, 6);
	//DrawCone(6, 2, 6);
	//DrawSphere(1, 15, 15);

	DrawFigure(alpha);

	if (this->showGrid)
		this->DrawGrid(10, 10, 10, 10);

	// -------------------------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	double stepA = 2 * PI / nSegAlpha;
	double stepB = 2 * PI / nSegAlpha;

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSegAlpha; i++) {
		for (int j = 0; j < nSegBeta; j++) {
			glVertex3f(r * cos(stepA * i) * cos(stepB * j), r + r * sin(stepA * i), r * cos(stepA * i) * sin(stepB * j));
			glVertex3f(r * cos(stepA * (i + 1)) * cos(stepB * j),r +  r * sin(stepA * (i + 1)), r * cos(stepA * (i + 1)) * sin(stepB * j));
		}
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	double step = 2 * PI / nSeg;

	//donja strana
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++) {
		glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
	}
	glEnd();

	//gornja strana
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++) {
		glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
	}
	glEnd();

	//stranice
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSeg+1; i++) {
		glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
		glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
	}
	glEnd();

}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	double step = 2 * PI / nSeg;
	//donja strana
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++) {
		glVertex3f(cos(step * i) * r, 0, sin(step * i) * r);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, h, 0);
	for (int i = 0; i < nSeg+1; i++) {
		glVertex3f(cos(step * i) * r, 0, sin(step * i) * r);
	}
	glEnd();

}

void CGLRenderer::DrawAxis(double width)
{
	glLineWidth(width);

	glBegin(GL_LINES);
		//x
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(axisLineLenght, 0, 0);
		//y
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, axisLineLenght, 0);
		//z
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, axisLineLenght);
	glEnd();

}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH)
{
	double startX = -width / 2;
	double startY = -height / 2;

	double stepX = width / nSegW;
	double stepY = height / nSegH;

	glBegin(GL_LINES);
	
	glColor3f(1, 1, 1);
	for (int i = 0; i < nSegW+1; i++) {
		glVertex3f(startX + stepX * i, 0, startY);
		glVertex3f(startX + stepX*i, 0, startY + height);
	}
	for (int i = 0; i < nSegH+1; i++) {
		glVertex3f(startX, 0, startY + stepY * i);
		glVertex3f(startX + width, 0, startY + stepY * i);
	}
		
	glEnd();
}

void CGLRenderer::DrawFigure(double alpha)
{
	double cylinderH = 0.4;
	double cylinderR = 0.1;

	glPushMatrix();

	//nacrtaj saksiju
	DrawCylinder(0.6, 0.8, 1, 8);
	glTranslatef(0, 0.6, 0);
	DrawCylinder(0.2, 1.1, 1.1, 8);
	
	glPushMatrix();

	glColor3f(0, 1, 0);
	DrawBranch(0, true);
	{
		//levo dete
		glPushMatrix();
		glColor3f(0, 1, 0);
		DrawBranch(30, false);
		{
			//levo dete
			glPushMatrix();
			glColor3f(1, 1, 0);
			DrawBranch(30 + alpha, false);
			{
				glPushMatrix();
				glColor3f(0, 1, 0);
				DrawBranch(30, true);
				glPopMatrix();

				glPushMatrix();
				glColor3f(0, 1, 0);
				DrawBranch(0, false);
				glPopMatrix();

				glPushMatrix();
				glColor3f(0, 1, 0);
				DrawBranch(-30, false);
				glPopMatrix();

			}
			glPopMatrix();

			//desno dete
			glPushMatrix();
			glColor3f(0, 1, 0);
			DrawBranch(-30, false);
			{
				glPushMatrix();
				glColor3f(0, 1, 0);
				DrawBranch(30, true);
				glPopMatrix();

				glPushMatrix();
				glColor3f(0, 1, 0);
				DrawBranch(-30, false);
				glPopMatrix();
			}
			glPopMatrix();

		}
		glPopMatrix();
		//srednje dete
		glPushMatrix();
		glColor3f(0, 1, 0);
		DrawBranch(0, true);
		glPopMatrix();
		//desno dete
		glPushMatrix();
		glColor3f(0, 1, 0);
		DrawBranch(-30, false);
		glPopMatrix();

	}
	glPopMatrix();

	glPopMatrix();
}

void CGLRenderer::DrawBranch(double angle, bool isCone)
{
	glRotatef(angle, 0.0, 0.0, 1.0);
	glTranslatef(0, 0.2, 0);
	if (isCone) {
		DrawCone(1, 0.3, 6);
	}
	else {
		DrawCylinder(1, 0.3, 0.3, 6);
	}
	glTranslatef(0, 1, 0);
	glColor3f(1, 1, 1);
	DrawSphere(0.2, 15, 15);
	glTranslatef(0, 0.2, 0);
}
