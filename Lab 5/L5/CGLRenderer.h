#pragma once
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

#include <GL\gl.h>
#include <GL\glu.h>

#define _USE_MATH_DEFINES
#define PI 3.14159
#include <math.h>
#include "CGLMaterial.h"

struct Color3f {
	float r;
	float g;
	float b;
};

struct Light6f {
	float x;
	float y;
	float z;

	float dx;
	float dy;
	float dz;
};

class CGLRenderer
{
private:
	HGLRC m_hrc;

	void CalculateViewPosition();
public:
	CGLRenderer();
	~CGLRenderer();

	bool showAxis = true;
	bool showGrid = true;
	bool showNormals = true;
	bool l1 = true;
	bool l2 = true;
	bool l3 = true;

	double lookingAt[3];
	double viewPosition[3];
	double upVector[3];

	double viewAngle[2];
	double viewR;

	double lookAtHeight = 2;

	double axisLineLenght = 10;

	double alpha = 0;
	
	Color3f colors[10];

	CGLMaterial materials[10];
	int numOfMat = 0;

	Light6f lights[10];

	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);
	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawCone(double h, double r, int nSeg);
	void DrawAxis(double width);
	void DrawGrid(double width, double height, int nSegW, int nSegH);
	void DrawStrip(double r1, double r2, double h, int nSeg);
	void DrawFigure(double alpha);
	void DrawPrism4s(double w, double h, double d, int nSegW, int nSegH, int nSegD);
	void DrawBranch(double angle = 0, bool isCone = false);

	void SetUpLights();
	void DisableLights();
	void EnableLights();
	void DrawWalls();
	void DrawWall();
	void DrawVase();
	void DrawPostolje();
};

