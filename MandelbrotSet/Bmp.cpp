#include "Bmp.h"

UINT BmpTrueColor24BufferSize(INT w, INT h)
{
	const INT rowSize = (w * 24 + 31) / 32 * 4;
	return sizeof(BitmapFileHeader)
		+ sizeof(BitmapInfoHeader)
		+ rowSize * h;
}

void BmpTrueColor24(INT w, INT h, int* data, void* buffer)
{
	BitmapFileHeader fileHeader;
	fileHeader.type = 19778;
	fileHeader.fileSize = BmpTrueColor24BufferSize(w, h);
	fileHeader.reserved1 = 0;
	fileHeader.reserved2 = 0;
	fileHeader.offsetToData = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

	BitmapInfoHeader infoHeader;
	infoHeader.headerSize = sizeof(BitmapInfoHeader);
	infoHeader.imageWidth = w;
	infoHeader.imageHeight = h;
	infoHeader.planesCount = 1;
	infoHeader.bitsPerPixel = 24;
	infoHeader.compression = 0;
	infoHeader.imageDataSize = 0;
	infoHeader.pxlPerMeterX = 0;
	infoHeader.pxlPerMeterY = 0;
	infoHeader.colorsCount = 0;
	infoHeader.colorsImportantCount = 0;

	memset(buffer, 0, fileHeader.fileSize);
	memcpy(buffer, &fileHeader, sizeof(fileHeader));
	memcpy((BYTE*)buffer + sizeof(fileHeader), &infoHeader, sizeof(infoHeader));

	BYTE* pixelData = (BYTE*)buffer + fileHeader.offsetToData;
	const INT rowSizeInBytes = (w * 3 + 3) / 4 * 4;
	for (INT y = 0; y < h; y++)
	{
		for (INT x = 0; x < w; x++)
		{
			const INT colorIndex = x + y * w;
			RgbQuad color;
			color.RgbColor = data[colorIndex];

			BYTE* colorInData = pixelData + 3 * x + (h - y - 1) * rowSizeInBytes;
			colorInData[0] = color.RgbStruct.rgbRed;
			colorInData[1] = color.RgbStruct.rgbGreen;
			colorInData[2] = color.RgbStruct.rgbBlue;
		}
	}
}