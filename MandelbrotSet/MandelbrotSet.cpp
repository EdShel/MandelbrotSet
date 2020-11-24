#include <iostream>
#include "Bmp.h"
#include <tchar.h>


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
		const INT w = 3, h = 3;
		int green = rgb(0, 255, 0);
		int blue = rgb(0, 0, 255);

		int imagePixels[w * h] = {
			green, green, green, 
			green, green, green, 
			green, green, blue 
		};

		const UINT bmpSize = getBmpTrueColor24BufferSize(w, h);
		BYTE* bmpImage = new BYTE[bmpSize];
		getBmpTrueColor24(w, h, imagePixels, bmpImage);

		WriteFile(file, bmpImage, bmpSize, NULL, NULL);

		CloseHandle(file);
		ShellExecute(NULL, NULL, fileName, NULL, NULL, 0);
	}
}
