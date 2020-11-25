#include <iostream>
#include <tchar.h>
#include "Bmp.h"
#include "MandelbrotSetCalculation.h"
#include "Colorization.h"
#include "SpeedTest.h"

bool SaveMandelbrotSetAsTrueColor24(const TCHAR* fileName, INT w, INT h, INT maxIterations, ColorizationFunc colorFunc)
{
	HANDLE file = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	INT* iterationsPerPixel = new INT[w * h];
	MandelbrotSetIterationsSequential(w, h, maxIterations, iterationsPerPixel);

	INT* pixelData = new INT[w * h];
	ColorizePixelIterations(iterationsPerPixel, pixelData, w * h, maxIterations, colorFunc);

	const UINT bmpSize = BmpTrueColor24BufferSize(w, h);
	BYTE* bmpImage = new BYTE[bmpSize];
	BmpTrueColor24(w, h, pixelData, bmpImage);

	WriteFile(file, bmpImage, bmpSize, NULL, NULL);

	CloseHandle(file);
	delete[] iterationsPerPixel;
	delete[] pixelData;
	delete[] bmpImage;
	return true;
}

bool SaveMandelbrotSetAsPalette8(const TCHAR* fileName, INT w, INT h, INT maxIterations, ColorizationFunc colorFunc)
{
	HANDLE file = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	INT* iterationsPerPixel = new INT[w * h];
	MandelbrotSetIterationsSequential(w, h, maxIterations, iterationsPerPixel);

	INT* paletteMap = new INT[maxIterations];
	for (int i = 0; i < maxIterations; i++)
	{
		paletteMap[i] = i;
	}
	INT* palette = new INT[maxIterations];
	ColorizePixelIterations(paletteMap, palette, maxIterations, maxIterations, colorFunc);

	const UINT bmpSize = BmpPalette8BufferSize(w, h);
	BYTE* bmpImage = new BYTE[bmpSize];
	BmpPalette8(w, h, palette, maxIterations, iterationsPerPixel, bmpImage);

	WriteFile(file, bmpImage, bmpSize, NULL, NULL);

	CloseHandle(file);
	delete[] iterationsPerPixel;
	delete[] paletteMap;
	delete[] palette;
	delete[] bmpImage;
	return true;
}

void Example()
{
	const INT w = 1024, h = 1024;
	const INT maxIterations = 128;
	const ColorizationFunc colorFunc = ColorRainbow;

	const TCHAR* bmpTrueColor = _T("C:\\Users\\Admin\\Desktop\\mandelbrotTrueColor.bmp");
	SaveMandelbrotSetAsTrueColor24(bmpTrueColor, w, h, maxIterations, colorFunc);
	ShellExecute(NULL, NULL, bmpTrueColor, NULL, NULL, 0);

	const TCHAR* bmpPalette = _T("C:\\Users\\Admin\\Desktop\\mandelbrotPalette.bmp");
	SaveMandelbrotSetAsPalette8(bmpPalette, w, h, maxIterations, colorFunc);
	ShellExecute(NULL, NULL, bmpPalette, NULL, NULL, 0);
}

int main()
{
	TestSpeed();
}
