#include <iostream>
#include <stdio.h>
#include <dos.h>
#include <Windows.h>

#define M_PI 3.14159265358979323846

#define OFFSETX 200
#define OFFSETY 200
#define OFFSETZ 30

const int spx = 10;
const int aa[9] = { spx ,-spx ,-spx,spx   ,  spx,-spx,-spx ,spx };
const int bb[9] = { spx ,spx  ,-spx,-spx  ,  spx,spx ,-spx ,-spx };
const int cc[9] = { -spx ,-spx ,-spx,-spx  ,  spx,spx ,spx  ,spx };

const int ff[12] = { 1,2,3,4,  5,6,7,8, 1,2,3,4 };
const int gg[12] = { 2,3,4,1,  6,7,8,5, 5,6,7,8 };

using namespace std;

HWND handle = GetConsoleWindow();
HDC dc = GetDC(handle);

int state = 0;
int red = 255;
int green = 0;
int blue = 0;

COLORREF getNextColour(float val)
{
	/*
	switch (state) {
	case 0:
	  green++;
	  if (green == 186) {
		green = 100;
		state = 1;
	  }
	  break;
	case 1:
	  red--;
	  if (red == 255) {
		red = 0;
		state = 2;
	  }
	  break;
	case 2:
	  blue++;
	  if (blue == 32) {
		blue = 31;
		state = 3;
	  }
	  break;
	case 3:
	  green--;
	  if (green == 255) {
		green = 0;
		state = 4;
	  }
	  break;
	case 4:
	  red++;
	  if (red == 32) {
		red = 31;
		state = 5;
	  }
	  break;
	case 5:
	  blue--;
	  if (blue == 255) {
		blue = 0;
		state = 0;
	  }
	  break;
	}
	*/

	//red = cos(val) * .5f + .5f;
	//green = cos(val - 2.f * M_PI / 3.f) * .5f + .5f;
	//blue = cos(val - 4.f * M_PI / 3.f) * .5f + .5f;

	red = sin(0.001f * val + 0) * 127 + 128;
	green = sin(0.001f * val + 2) * 127 + 128;
	blue = sin(0.001f * val + 4) * 127 + 128;

	//red *= 255.0f;
	//green *= 255.0f;
	//blue *= 255.0f;

	return RGB(red, green, blue);
}

void drawLine(int ax, int ay, int bx, int by, HPEN pen)
{
	SelectObject(dc, pen);
	MoveToEx(dc, ax, ay, NULL);
	LineTo(dc, bx, by);
}

void cube(float rotx, float roty, float rotz, HPEN pen);

int main()
{
	HPEN clearPen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));
	HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

	float x = 0;
	uint32_t v = 0;
	while (true)
	{
		x += 0.005f;

		v += 3;
		if (v >= (uint32_t)-1) v = 0;

		HPEN pen = CreatePen(PS_SOLID, 2, getNextColour(v));
		cube(0, x, 0, pen);
		DeleteObject(pen);

		Sleep(5);
		cube(0, x, 0, clearPen);
	}

	cin.get();
	cin.get();
}

void cube(float rotx, float roty, float rotz, HPEN pen)
{
	int newx[8]; //Jakas czarna magia (znalazle te wzory w internecie i napisalem ten skrypt) sam do konca nie ogarniam tego jak to dziala, ale wazne ze dziala
	int newy[8];
	int i, ii, loopi;
	float xt, yt, zt, x, y, z, sinax, cosax, sinay, cosay, sinaz, cosaz;

	int sx, sy, ex, ey;
	int vertex;

	float xpos = 0;
	float ypos = 0;
	float zpos = 0;

	int numdraw = 1;

	xpos = xpos + 0.0;
	ypos = ypos + 0.0;
	zpos = zpos + 0.0;

	//rotx=rotx+.1;             
	//roty=roty+.1;             
	//rotz=rotz+0;             


	sinax = sin(rotx);
	cosax = cos(rotx);

	sinay = sin(roty);
	cosay = cos(roty);

	sinaz = sin(rotz);
	cosaz = cos(rotz);

	for (i = 0; i < 8; i++)  //Przeksztalcam pozycje 3D w 2D (Robie projekcje rogow szeszcianu na ekran)
	{
		x = aa[i];
		y = bb[i];
		z = cc[i];

		yt = y * cosax - z * sinax;
		zt = y * sinax + z * cosax;
		y = yt;
		z = zt;

		xt = x * cosay - z * sinay;
		zt = x * sinay + z * cosay;
		x = xt;
		z = zt;

		xt = x * cosaz - y * sinaz;
		yt = x * sinaz + y * cosaz;
		x = xt;
		y = yt;

		x = x + xpos;
		y = y + ypos;
		z = z + OFFSETZ - zpos;

		newx[i] = (x * 64 / z) + OFFSETX;
		newy[i] = (y * 64 / z) + OFFSETY;
	}

	for (ii = 0; ii < 12; ii++)  //Rysujemy 12 linij (verteksy szeszciana)
	{
		vertex = ff[ii] - 1;
		sx = newx[vertex];
		sy = newy[vertex];

		vertex = gg[ii] - 1;
		ex = newx[vertex];
		ey = newy[vertex];
		int sz = 3;

		drawLine(sx, sy, ex, ey, pen);
	}
}