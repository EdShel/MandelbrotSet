#include <iostream>
#include "Bmp.cpp"
#include <tchar.h>

int main()
{
	HANDLE file = CreateFile(
		_T("C:\\Users\\Admin\\Desktop\\test.bmp"),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL);
	if (file != INVALID_HANDLE_VALUE)
	{
		BitmapFileHeader fileHeader;
		fileHeader.type = 19778;
		fileHeader.reserved1 = 0;
		fileHeader.reserved2 = 0;

		BitmapInfoHeader infoHeader;
		infoHeader.headerSize = sizeof(BitmapInfoHeader);
		infoHeader.imageWidth = 4;
		infoHeader.imageHeight = 4;
		infoHeader.planesCount = 1;
		infoHeader.bitsPerPixel = 8;
		infoHeader.compression = 0;
		infoHeader.imageDataSize = 0;
		infoHeader.pxlPerMeterX = 0;
		infoHeader.pxlPerMeterY = 0;
		infoHeader.colorsCount = 2; //0;
		infoHeader.colorsImportantCount = 0;

		RgbQuad yellow; 
		yellow.RgbColor = 0xe8eb34;
		RgbQuad blue;
		blue.RgbColor = 0x0000FF;
		UINT colorTableSize = 2;//1 << infoHeader.bitsPerPixel;
		RgbQuad* colorTable = new RgbQuad[colorTableSize];
		colorTable[0] = yellow;
		colorTable[1] = blue;

		BYTE pixelData[16] = {
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 1, 1,
			1, 1, 1, 1
		};

		fileHeader.offsetToData = sizeof(fileHeader)
			+ sizeof(infoHeader)
			+ sizeof(RgbQuad) * (colorTableSize);
		fileHeader.fileSize = fileHeader.offsetToData + sizeof(pixelData);

		WriteFile(file, &fileHeader, sizeof(fileHeader), NULL, NULL);
		WriteFile(file, &infoHeader, sizeof(infoHeader), NULL, NULL);
		WriteFile(file, colorTable, sizeof(RgbQuad) * colorTableSize, NULL, NULL);
		WriteFile(file, pixelData, sizeof(pixelData), NULL, NULL);

		CloseHandle(file);
		delete[] colorTable;
	}
}
