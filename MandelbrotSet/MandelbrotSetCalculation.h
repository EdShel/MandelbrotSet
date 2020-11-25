#pragma once

typedef void(*MandelbrotFunc)(INT w, INT h, INT maxIterations, INT * iterationsPerPoint);

void MandelbrotSetIterationsSequential(INT w, INT h, INT maxIterations, INT * iterationsPerPoint);

void MandelbrotSetIterationsSequentialOptimized(INT w, INT h, INT maxIterations, INT * iterationsPerPoint);
