#pragma once

#define Rgb(r, g, b) ((b << 16) | (g << 8) | r)

#define RgbGetR(col) (col & 0xFF)

#define RgbGetG(col) ((col >> 8) & 0xFF)

#define RgbGetB(col) ((col >> 16) & 0xFF)

typedef int(*ColorizationFunc)(double completedIterationsPercent);

int hsv(double hue, double saturation, double value);

int ColorRainbow(double percent);

int ColorBlueLightning(double percent);

int ColorGrayscale(double percent);

int ColorCanonical(double percent);

void ColorizePixelIterations(INT * iterationsPerPixel, INT * colors, UINT pixelsCount, INT maxIteration, ColorizationFunc colorizator);
