#pragma once

typedef int(*ColorizationFunc)(double completedIterationsPercent);

int ColorRainbow(double percent);

int ColorBlueLightning(double percent);

int ColorGrayscale(double percent);

int ColorCanonical(double percent);

void ColorizePixelIterations(INT * iterationsPerPixel, INT * colors, UINT pixelsCount, INT maxIteration, ColorizationFunc colorizator);
