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

	this->upVector[1] = cos(viewAngle[0]) < 0 ? -1 : 1;
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

	colors[0] = { 1.0, 0.0, 1.0 };
	colors[1] = { 0.0, 1.0, 1.0 };
	colors[2] = { 0.0, 0.0, 1.0 };
	colors[3] = { 1.0, 0.0, 0.0 };
	colors[4] = { 1.0, 0.0, 1.0 };

	materials[0] = CGLMaterial();
	{
		CGLMaterial* mat = &materials[0];
		Color3f col = colors[0];
		mat->SetAmbient(col.r, col.g, col.b);
		mat->SetDiffuse(min(col.r * 0.4 + 0.4, 1.0), min(col.g * 0.4 + 0.4, 1.0), min(col.b * 0.4 + 0.4, 1.0));
		mat->SetSpecular(min(col.r * 0.6 + 0.4, 1.0), min(col.g * 0.6 + 0.4, 1.0), min(col.b * 0.6 + 0.4, 1.0));
		mat->SetShininess(100);
	}
	numOfMat++;
	materials[1] = CGLMaterial();
	{
		CGLMaterial* mat = &materials[1];
		Color3f col = colors[1];
		mat->SetAmbient(col.r,col.g, col.b);
		mat->SetDiffuse(0.6, 0.6, 0.6);
		mat->SetSpecular(1.0, 1.0, 1.0, 1.0);
		mat->SetShininess(0);
	}
	numOfMat++;
	materials[2] = CGLMaterial();
	{
		CGLMaterial* mat = &materials[2];
		mat->SetAmbient(0.8, 0.8, 0.8, 1.0);
		mat->SetDiffuse(0.9, 0.9, 0.9);
		mat->SetSpecular(0.9, 0.9, 0.9, 1.0);
		mat->SetShininess(128.0);
		//mat->SetEmission(0.5, 0.5, 0.5, 1.0);
	}
	numOfMat++;

	lights[0] = { 0.0, 19, 0.0 , 0.0, -1.0, 0.0 };
	lights[1] = { 9, 10, 0.0 , -1.0, 0.0, 0.0 };
	lights[2] = { -9.0, 10, 0.0 , 1.0, 0.0, 0.0};
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
	gluPerspective(45, (double)w / (double)h, 1, 100);
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

	bool norm = showNormals;
	showNormals = false;

	SetUpLights();


	showNormals = norm;

	DrawFigure(alpha);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHTING);

	if (this->showGrid)
		this->DrawGrid(10, 10, 10, 10);

	// -------------------------------------------
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	nSegAlpha = max(4, nSegAlpha);
	nSegBeta = max(4, nSegBeta);
	double stepA = 2 * PI / nSegAlpha;
	double stepB = 2 * PI / nSegAlpha;

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSegAlpha; i++) {
		for (int j = 0; j < nSegBeta; j++) {
			glNormal3f(cos(stepA * i) * cos(stepB * j), sin(stepA * i), cos(stepA * i) * sin(stepB * j));
			glVertex3f(r * cos(stepA * i) * cos(stepB * j), r + r * sin(stepA * i), r * cos(stepA * i) * sin(stepB * j));

			glNormal3f(cos(stepA * (i + 1)) * cos(stepB * j), sin(stepA * (i + 1)), cos(stepA * (i + 1)) * sin(stepB * j));
			glVertex3f(r * cos(stepA * (i + 1)) * cos(stepB * j),r +  r * sin(stepA * (i + 1)), r * cos(stepA * (i + 1)) * sin(stepB * j));
		}
	}
	glEnd();
	if (showNormals) {
		DisableLights();
		glBegin(GL_LINES);
		glColor3f(1, 1, 0);
		for (int i = 0; i < nSegAlpha; i++) {
			for (int j = 0; j < nSegBeta; j++) {
				glVertex3f(r * cos(stepA * i) * cos(stepB * j), r + r * sin(stepA * i), r * cos(stepA * i) * sin(stepB * j));
				glVertex3f((r+1) * cos(stepA * i) * cos(stepB * j), r + (r+1) * sin(stepA * i), (r+1) * cos(stepA * i) * sin(stepB * j));

				glVertex3f(r * cos(stepA * (i + 1)) * cos(stepB * j), r + r * sin(stepA * (i + 1)), r * cos(stepA * (i + 1)) * sin(stepB * j));
				glVertex3f((r + 1) * cos(stepA * (i + 1)) * cos(stepB * j), r + (r + 1) * sin(stepA * (i + 1)), (r + 1) * cos(stepA * (i + 1)) * sin(stepB * j));

			}
		}
		glEnd();
		EnableLights();
	}
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	double step = 2 * PI / nSeg;

	//donja strana
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++) {
		glNormal3f(0, -1, 0);
		glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
	}
	glEnd();

	//gornja strana
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++) {
		glNormal3f(0, 1, 0);
		glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
	}
	glEnd();

	//stranice
	int v = r2 > r1 ? -1 : 1; 
	double r = abs(r1 - r2);
	double L = sqrt(r * r + h * h);
	double ny = r / L;
	double nr = h / L;
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < nSeg+1; i++) {
		glNormal3f(nr * cos(step*i), ny*v, nr*sin(step*i));
		glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
		glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
	}
	glEnd();

	if (this->showNormals) {
		DisableLights();
		glBegin(GL_LINES);
		glColor3f(1, 1, 0);
		for (int i = 0; i < nSeg+1; i++) {
			//donja strana
			glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
			glVertex3f(cos(step * i) * r1, -1, sin(step * i) * r1);
			//gornja strana 
			glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
			glVertex3f(cos(step * i) * r2, h+1, sin(step * i) * r2);

			//tacka dole
			glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
			glVertex3f(cos(step * i) * r1 + cos(step * i) * nr, ny*v, sin(step * i) * r1 + sin(step * i) * nr);
			//tacka gore
			glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
			glVertex3f(cos(step * i) * r2 + cos(step * i) * nr, h + ny*v, sin(step * i) * r2 + sin(step * i)*nr);
		}

		glEnd();
		EnableLights();
	}

}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	double step = 2 * PI / nSeg;

	double L = sqrt(r * r + h * h);
	double ny = r / L;
	double nr = h / L;

	//donja strana
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < nSeg; i++) {
		glNormal3f(0, -1, 0);
		glVertex3f(cos(step * i) * r, 0, sin(step * i) * r);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, ny, 0);

	glVertex3f(0, h, 0);
	for (int i = 0; i < nSeg+1; i++) {
		glNormal3f(nr * cos(step * i), ny, nr * sin(step * i));
		glVertex3f(cos(step * i) * r, 0, sin(step * i) * r);
	}
	glEnd();

	if (this->showNormals) {
		DisableLights();
		glBegin(GL_LINES);
		glColor3f(1, 1, 0);
		for (int i = 0; i < nSeg + 1; i++) {
			//donja strana
			glVertex3f(cos(step * i) * r, 0, sin(step * i) * r);
			glVertex3f(cos(step * i) * r, -1, sin(step * i) * r);
			//tacka dole
			glVertex3f(cos(step * i) * r, 0, sin(step * i) * r);
			glVertex3f(cos(step * i) * r + cos(step * i) * nr, ny, sin(step * i) * r + sin(step * i) * nr);
			//tacka gore
			glVertex3f(0, h, 0);
			glVertex3f(cos(step * i) + cos(step * i) * nr, h + ny, sin(step * i) + sin(step * i) * nr);
		}

		glEnd();
		EnableLights();
	}

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

void CGLRenderer::DrawStrip(double r1, double r2, double h, int nSeg)
{
	double step = 2 * PI / nSeg;

	int v = r2 > r1 ? -1 : 1;
	double r = abs(r1 - r2);
	double L = sqrt(r * r + h * h);
	double ny = r / L;
	double nr = h / L;

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSeg; i++) {
		glNormal3f(nr * cos(step * i), ny * v, nr * sin(step * i));
		glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
		glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
	}
	glEnd();

	if (showNormals) {
		DisableLights();
		glBegin(GL_LINES);
		glColor3f(1, 1, 0);
		for (int i = 0; i < nSeg + 1; i++) {

			//tacka dole
			glVertex3f(cos(step * i) * r1, 0, sin(step * i) * r1);
			glVertex3f(cos(step * i) * r1 + cos(step * i) * nr, ny * v, sin(step * i) * r1 + sin(step * i) * nr);
			//tacka gore
			glVertex3f(cos(step * i) * r2, h, sin(step * i) * r2);
			glVertex3f(cos(step * i) * r2 + cos(step * i) * nr, h + ny * v, sin(step * i) * r2 + sin(step * i) * nr);
		}

		glEnd();
		EnableLights();
	}
}

void CGLRenderer::DrawFigure(double alpha)
{
	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	DrawWalls();
	glDisable(GL_CULL_FACE);
	DrawPostolje();
	DrawVase();

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

void CGLRenderer::SetUpLights()
{
	if (l1) {
		glPushMatrix();
		glColor3f(colors[2].r, colors[2].g, colors[2].b);
		glTranslatef(lights[0].x, lights[0].y, lights[0].z);
		DrawSphere(0.2, 12, 12);
		glPopMatrix();
	}if (l2) {
		glPushMatrix();
		glColor3f(colors[3].r, colors[3].g, colors[3].b);
		glTranslatef(lights[1].x, lights[1].y, lights[1].z);
		DrawSphere(0.2, 12, 12);
		glPopMatrix();
	}if (l3) {
		glPushMatrix();
		glColor3f(colors[4].r, colors[4].g, colors[4].b);
		glTranslatef(lights[2].x, lights[2].y, lights[2].z);
		DrawSphere(0.2, 12, 12);
		glPopMatrix();
	}
	glEnable(GL_LIGHTING);

	GLfloat amb[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	if (l1) {
		GLfloat light1_ambient[] = { 0.0, 0.0, 1.0, 1.0 };
		GLfloat light1_diffuse[] = { 0.0, 0.0, 0.4, 1.0 };
		GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light1_position[] = { lights[0].x, lights[0].y, lights[0].z, 1.0 };
		GLfloat spot_direction[] = { lights[0].dx, lights[0].dy, lights[0].dz };
		glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.1);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.04);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 20.0);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.2);
		glEnable(GL_LIGHT0);
	}
	if (l2) {
		GLfloat light1_ambient[] = { 0, 1.0, 0.0, 1.0 };
		GLfloat light1_diffuse[] = { 0.0, 0.4, 0.0, 1.0 };
		GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light1_position[] = { lights[1].x, lights[1].y, lights[1].z, 1.0 };
		GLfloat spot_direction[] = { lights[1].dx, lights[1].dy, lights[1].dz };
		glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
		glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.1);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.03);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.2);
		glEnable(GL_LIGHT1);
	}
	if (l3) {
		GLfloat light1_ambient[] = { 1.0, 0, 0, 1.0 };
		GLfloat light1_diffuse[] = { 0.4, 0.0, 0.0, 1.0 };
		GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light1_position[] = { lights[2].x, lights[2].y, lights[2].z, 1.0 };
		GLfloat spot_direction[] = { lights[2].dx, lights[2].dy, lights[2].dz };
		glLightfv(GL_LIGHT2, GL_AMBIENT, light1_ambient);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light1_diffuse);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light1_specular);
		glLightfv(GL_LIGHT2, GL_POSITION, light1_position);
		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.1);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.1);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.03);
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.2);
		glEnable(GL_LIGHT2);
	}

	GLfloat light1_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { 0.0, 25.0, 0.0, 1.0 };
	GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT3, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT3, GL_POSITION, light1_position);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.01);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.01);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0);
	glEnable(GL_LIGHT3);
}

void CGLRenderer::DrawPrism4s(double w, double h, double d, int nSegW, int nSegH, int nSegD)
{
	//gore i dole
	glBegin(GL_QUADS);
	for (float x = -w / 2; x < w / 2 - w / nSegW; x += w / nSegW) {
		for (float z = -d / 2; z < d / 2 - d / nSegD; z += d / nSegD) {
			glNormal3f(0, 1.0, 0);
			glVertex3f(x, h / 2, z);
			glVertex3f(x + w / nSegW, h / 2, z);
			glVertex3f(x + w / nSegW, h / 2, z + d / nSegD);
			glVertex3f(x, h / 2, z + d/nSegD);

			glNormal3f(0, -1.0, 0);
			glVertex3f(x, -h / 2, z);
			glVertex3f(x + w / nSegW, -h / 2, z);
			glVertex3f(x + w / nSegW, -h / 2, z + d / nSegD);
			glVertex3f(x, -h / 2, z + d / nSegD);
		}
	}
	glEnd();
	//stranice

	glBegin(GL_QUADS);

	for (float x = -w / 2; x < w / 2 - w / nSegW; x += w / nSegW) {
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(x, h / 2, -d / 2);
		glVertex3f(x + (w / nSegW), h / 2, -d / 2);
		glVertex3f(x + (w / nSegW), -h / 2, -d / 2);
		glVertex3f(x, -h / 2, -d / 2);

		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(x, h / 2, d / 2);
		glVertex3f(x + w / nSegW, h / 2, d / 2);
		glVertex3f(x + w / nSegW, -h / 2, d / 2);
		glVertex3f(x, -h / 2, d / 2);

	}
	for (float z = -d / 2; z < d / 2 - d / nSegD; z += d / nSegD) {
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-w / 2, h / 2, z);
		glVertex3f(-w / 2, h / 2, z + d / nSegD);
		glVertex3f(-w / 2, -h / 2, z + d / nSegD);
		glVertex3f(-w / 2, -h / 2, z);

		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(w / 2, h / 2, z);
		glVertex3f(w / 2, h / 2, z + d / nSegD);
		glVertex3f(w / 2, -h / 2, z + d / nSegD);
		glVertex3f(w / 2, -h / 2, z);
	}

	glEnd();
}

void CGLRenderer::DrawWalls()
{
	glPushMatrix();
	materials[2].Select(GL_FRONT_AND_BACK);
	glTranslatef(0.0, 20.0, 0.0);
	DrawWall();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	DrawWall();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	DrawWall();
	glRotatef(90.0, 0.0, 1.0, 0.0);
	DrawWall();
	glRotatef(90.0, 0.0, 0.0, 1.0);
	DrawWall();
	glRotatef(180.0, 0.0, 0.0, 1.0);
	DrawWall();
	glPopMatrix();
}

void CGLRenderer::DisableLights()
{
	glDisable(GL_LIGHTING);
}

void CGLRenderer::DrawWall()
{
	glBegin(GL_QUADS);
	for (float y = -20; y <= 20; y += 0.2) {
		for (float z = -20; z <= 20; z += 0.2) {
			glNormal3f(-1.0, 0.0, 0.0);
			glVertex3f(20, y, z);
			glVertex3f(20, y+0.2, z);
			glVertex3f(20, y + 0.2, z +0.2);
			glVertex3f(20, y, z + 0.2);
		}
	}
	glEnd();
}

void CGLRenderer::EnableLights()
{
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawVase()
{
	materials[0].Select(GL_FRONT);
	DrawStrip(2, 1.5, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[1].Select(GL_FRONT);
	DrawStrip(1.5, 1, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[0].Select(GL_FRONT);
	DrawStrip(1, 1, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[1].Select(GL_FRONT);
	DrawStrip(1, 1, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[0].Select(GL_FRONT);
	DrawStrip(1, 1.5, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[1].Select(GL_FRONT);
	DrawStrip(1.5, 1.0, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[0].Select(GL_FRONT);
	DrawStrip(1, 1.5, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[1].Select(GL_FRONT);
	DrawStrip(1.5, 2.0, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[0].Select(GL_FRONT);
	DrawStrip(2, 1.5, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[1].Select(GL_FRONT);
	DrawStrip(1.5, 1, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[0].Select(GL_FRONT);
	DrawStrip(1, 1.5, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[1].Select(GL_FRONT);
	DrawStrip(1.5, 1.0, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[0].Select(GL_FRONT);
	DrawStrip(1, 1.5, 0.5, 10);
	glTranslatef(0, 0.5, 0);

	materials[1].Select(GL_FRONT);
	DrawStrip(1.5, 2.0, 0.5, 10);
	glTranslatef(0, 0.5, 0);
}

void CGLRenderer::DrawPostolje()
{
	bool sn = showNormals;
	showNormals = false;
	materials[2].Select();
	glTranslatef(0, -2, 0);
	DrawSphere(2, 12, 12);
	glTranslatef(0, 2, 0);
	DrawCylinder(4, 1, 1, 12);
	glTranslated(0, 4, 0);
	DrawPrism4s(6, 1, 6, 20, 10, 20);
	glTranslatef(0, 0.5, 0);

	showNormals = sn;
}
