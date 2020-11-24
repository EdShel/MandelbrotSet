#include <iostream>
#include <tchar.h>
#include "Bmp.h"
#include "MandelbrotSetCalculation.h"
#include "Colorization.h"

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
		const INT maxIterations = 512;
		const ColorizationFunc colorFunc = ColorBlueLightning;

		INT* iterationsPerPixel = new INT[w * h];
		MandelbrotSetIterationsSequential(w, h, maxIterations, iterationsPerPixel);

		INT* pixelData = new INT[w * h];
		ColorizePixelIterations(iterationsPerPixel, pixelData, w * h, maxIterations, colorFunc);

		const UINT bmpSize = BmpTrueColor24BufferSize(w, h);
		BYTE* bmpImage = new BYTE[bmpSize];
		BmpTrueColor24(w, h, pixelData, bmpImage);

		WriteFile(file, bmpImage, bmpSize, NULL, NULL);

		delete[] iterationsPerPixel;
		delete[] pixelData;
		CloseHandle(file);

		ShellExecute(NULL, NULL, fileName, NULL, NULL, 0);
	}
}
