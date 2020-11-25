#include "Bmp.h"
#include "Colorization.h"
#include <math.h>

UINT BmpTrueColor24BufferSize(INT w, INT h)
{
	const INT rowSize = (w * 24 + 31) / 32 * 4;
	return sizeof(BitmapFileHeader)
		+ sizeof(BitmapInfoHeader)
		+ rowSize * h;
}

void BmpTrueColor24(INT w, INT h, INT* data, void* buffer)
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

INT BmpPalette8RowSizeBytes(INT rowWidth)
{
	return (rowWidth + 3) / 4 * 4;
}

UINT BmpPalette8BufferSize(INT w, INT h)
{
	const INT rgbQuadTableSize = (1 << 8) * sizeof(RgbQuad);
	return sizeof(BitmapFileHeader)
		+ sizeof(BitmapInfoHeader)
		+ rgbQuadTableSize
		+ BmpPalette8RowSizeBytes(w) * h;
}

void BmpPalette8(INT w, INT h, INT* colorsPalette, UINT paletteSize, INT* colorIndices, void* buffer)
{
	const INT colorsCount = min((1 << 8) - 1, paletteSize);
	BitmapFileHeader fileHeader;
	fileHeader.type = 19778;
	fileHeader.fileSize = BmpPalette8BufferSize(w, h);
	fileHeader.reserved1 = 0;
	fileHeader.reserved2 = 0;
	fileHeader.offsetToData =
		sizeof(BitmapFileHeader)
		+ sizeof(BitmapInfoHeader)
		+ colorsCount * sizeof(RgbQuad);

	BitmapInfoHeader infoHeader;
	infoHeader.headerSize = sizeof(BitmapInfoHeader);
	infoHeader.imageWidth = w;
	infoHeader.imageHeight = h;
	infoHeader.planesCount = 1;
	infoHeader.bitsPerPixel = 8;
	infoHeader.compression = 0;
	infoHeader.imageDataSize = 0;
	infoHeader.pxlPerMeterX = 0;
	infoHeader.pxlPerMeterY = 0;
	infoHeader.colorsCount = colorsCount;
	infoHeader.colorsImportantCount = colorsCount;

	memset(buffer, 0, fileHeader.fileSize);
	memcpy(buffer, &fileHeader, sizeof(fileHeader));
	memcpy((BYTE*)buffer + sizeof(fileHeader), &infoHeader, sizeof(infoHeader));

	RgbQuad* paletteData = (RgbQuad*)((BYTE*)buffer + sizeof(fileHeader) + sizeof(infoHeader));
	const double takenColorStep = (double)paletteSize / colorsCount;
	for (UINT i = 0; i < colorsCount; i++)
	{
		const int colorToInclude = colorsPalette[(int)(i * takenColorStep)];
		paletteData[i].RgbStruct.rgbRed = RgbGetR(colorToInclude);
		paletteData[i].RgbStruct.rgbGreen = RgbGetG(colorToInclude);
		paletteData[i].RgbStruct.rgbBlue = RgbGetB(colorToInclude);
		paletteData[i].RgbStruct.rgbReserved = 0;
	}

	BYTE* pixelData = (BYTE*)buffer + fileHeader.offsetToData;
	const INT rowSizeInBytes = BmpPalette8RowSizeBytes(w);
	for (INT y = 0; y < h; y++)
	{
		for (INT x = 0; x < w; x++)
		{
			const INT pixelIndex = x + y * w;
			const INT colorIndexInGivenPalette = colorIndices[pixelIndex];
			BYTE colorIndexInSavedPalette = (BYTE)ceil(colorIndexInGivenPalette / takenColorStep);

			BYTE* pixelInData = pixelData + x + (h - y - 1) * rowSizeInBytes;
			*pixelInData = colorIndexInSavedPalette;
		}
	}
}