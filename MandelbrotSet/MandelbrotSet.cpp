#include <iostream>
#include "Bmp.h"
#include <tchar.h>

void getMandelbrotSet(INT* buffer, INT w, INT h)
{
	const double left = -2.25;
	const double right = 0.75;
	const double top = -1.5;
	const double bottom = 1.5;

	const double xStep = (right - left) / w;
	const double yStep = (bottom - top) / h;

	const int maxIterations = 100;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			const double pointX = left + x * xStep;
			const double pointY = top + y * yStep;

			double zX = pointX;
			double zY = pointY;
			int iterations = 0;
			while (iterations < maxIterations)
			{
				double zX2 = zX * zX - zY * zY;
				double zY2 = 2 * zX * zY;

				iterations++;
				//zX = (zX * zX) - (zY * zY) + pointX;
				//zY = (2 * tempZX * zY) + pointY;
				zX = zX2 + pointX;
				zY = zY2 + pointY;

				if (abs(zX + zY) > 4)
				{
					break;
				}
			}

			int bright = (int)(255 * (double)iterations / maxIterations) % 256;
			buffer[x + y * w] = rgb(iterations % 255, iterations & 0x12, bright);
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
		const INT w = 4096, h = 4096;
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
