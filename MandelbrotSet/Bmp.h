#pragma once
#include <Windows.h>

#pragma pack(push, 1)
struct BitmapFileHeader
{
	INT16 type;
	INT32 fileSize;
	INT16 reserved1;
	INT16 reserved2;
	INT32 offsetToData;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BitmapInfoHeader
{
	INT32 headerSize;
	INT32 imageWidth;
	INT32 imageHeight;
	INT16 planesCount;
	INT16 bitsPerPixel;
	INT32 compression;
	INT32 imageDataSize;
	INT32 pxlPerMeterX;
	INT32 pxlPerMeterY;
	INT32 colorsCount;
	INT32 colorsImportantCount;
};
#pragma pack(pop)

union RgbQuad
{
	struct
	{
		BYTE rgbBlue;
		BYTE rgbGreen;
		BYTE rgbRed;
		BYTE rgbReserved;
	} RgbStruct;
	INT32 RgbColor;
};

UINT BmpTrueColor24BufferSize(INT w, INT h);

void BmpTrueColor24(INT w, INT h, int * data, void * buffer);
