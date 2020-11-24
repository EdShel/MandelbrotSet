#include <iostream>
#include <Windows.h>
#include "Colorization.h"

#define rgb(r, g, b) ((b << 16) | (g << 8) | r)

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

int ColorRainbow(double percent)
{
	const int cycles = 30;
	const double percentScaled = 4 * (0.5 + percent) * (0.5 + percent);
	return hsv((int)(percentScaled * 360 * cycles + 270) % 360, 0.7f, 0.9f);
}

int ColorBlueLightning(double percent)
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

int ColorGrayscale(double percent)
{
	int brightness = (int)(255 * percent);
	return rgb(brightness, brightness, brightness);
}

int ColorCanonical(double percent)
{
	if (percent >= 1)
	{
		return rgb(0, 0, 0);
	}
	return rgb(255, 255, 255);
}

void ColorizePixelIterations(INT* iterationsPerPixel, INT* resultColorsBuffer, UINT pixelsCount, INT maxIteration, ColorizationFunc colorizator)
{
	const double maxIterationDouble = (double)maxIteration;
	for (UINT i = 0; i < pixelsCount; i++)
	{
		const double percent = iterationsPerPixel[i] / maxIterationDouble;
		resultColorsBuffer[i] = colorizator(percent);
	}
}