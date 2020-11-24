#include <iostream>
#include "Bmp.h"
#include <tchar.h>

int hsv(float hue, float saturation, float value)
{
	float C = saturation * value;
	float X = C * (1 - abs(fmod(hue / 60.0, 2) - 1));
	float m = value - C;
	float r, g, b;
	if (hue >= 0 && hue < 60)
	{
		r = C, g = X, b = 0;
	}
	else if (hue >= 60 && hue < 120)
	{
		r = X, g = C, b = 0;
	}
	else if (hue >= 120 && hue < 180)
	{
		r = 0, g = C, b = X;
	}
	else if (hue >= 180 && hue < 240)
	{
		r = 0, g = X, b = C;
	}
	else if (hue >= 240 && hue < 300)
	{
		r = X, g = 0, b = C;
	}
	else
	{
		r = C, g = 0, b = X;
	}

	int R = (r + m) * 255;
	int G = (g + m) * 255;
	int B = (b + m) * 255;

	return rgb(R, G, B);
}

int getColorRainbow(double percent)
{
	const int cycles = 30;
	const double percentScaled = 4 * (0.5 + percent) * (0.5 + percent);
	return hsv((int)(percentScaled * 360 * cycles + 270) % 360, 0.7f, 0.9f);
}

int getColorForIterationPercent(double percent)
{
	if (percent >= 1.)
	{
		return rgb(0, 0, 0);
	}

	if (percent <= 0.1)
	{
		return rgb(
			(int)(percent * 10),
			0,
			(int)(percent / 0.1 * 255));
	}
	else if (percent <= 0.5)
	{
		return rgb(
			10 + (int)(percent * 240),
			(int)(percent / 0.5 * 255), 0);
	}
	else
	{
		return rgb(
			(int)(percent * 255),
			(int)(percent * 255),
			(int)(percent * 255));
	}
}

int getColorGray(double percent)
{
	int brightness = (int)(255 * percent);
	return rgb(brightness, brightness, brightness);
}

#define MANDELBROT_LEFT -2.25
#define MANDELBROT_RIGHT 0.75
#define MANDELBROT_TOP -1.5
#define MANDELBROT_BOTTOM 1.5

void getMandelbrotSet(INT* buffer, INT w, INT h)
{
	const double xStep = (MANDELBROT_RIGHT - MANDELBROT_LEFT) / w;
	const double yStep = (MANDELBROT_BOTTOM - MANDELBROT_TOP) / h;

	const int maxIterations = 512;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			const double pointX = MANDELBROT_LEFT + x * xStep;
			const double pointY = MANDELBROT_RIGHT + y * yStep;

			double zX = pointX;
			double zY = pointY;
			int iterations = 0;
			for (; zX * zX + zY * zY < 4 && iterations < maxIterations; iterations++)
			{
				// z0 = c = x + iy
				// z1 = z0 * z0 + c = (x + iy)(x + iy) + c =
				// = x^2 + 2xyi + y^2i^2 + c =
				// = (x^2 - y^2) + (2xy)i + c
				double zXSquare = zX * zX - zY * zY;
				double zYSquare = 2 * zX * zY;

				zX = zXSquare + pointX;
				zY = zYSquare + pointY;
			}
			double percent = (double)iterations / maxIterations;
			buffer[x + y * w] = getColorRainbow(percent);
		}
	}

}

int main()
{
	const TCHAR* fileName = _T("C:\\Users\\Admin\\Desktop\\test.bmp");
	HANDLE file = CreateFile(
		fileName,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL);
	if (file != INVALID_HANDLE_VALUE)
	{
		const INT w = 1000, h = 1000;
		int green = rgb(0, 255, 0);
		int blue = rgb(0, 0, 255);

		int* imagePixels = new int[w * h];
		getMandelbrotSet(imagePixels, w, h);

		const UINT bmpSize = getBmpTrueColor24BufferSize(w, h);
		BYTE* bmpImage = new BYTE[bmpSize];
		getBmpTrueColor24(w, h, imagePixels, bmpImage);

		WriteFile(file, bmpImage, bmpSize, NULL, NULL);

		delete[] imagePixels;
		CloseHandle(file);
		ShellExecute(NULL, NULL, fileName, NULL, NULL, 0);
	}
}
