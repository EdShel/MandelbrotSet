#include <Windows.h>
#include <iostream>
#include <omp.h>
#include "MandelbrotSetCalculation.h"
#include "SpeedTest.h"

void PrintAbsoluteSpeed(INT w, INT h, INT maxIterations, INT testsCount, MandelbrotFunc func)
{
	INT* iterationsPerPixel = new INT[w * h];
	double minTime = DBL_MAX;
	for (int i = 0; i < testsCount; i++)
	{
		const double begin = omp_get_wtime();
		func(w, h, maxIterations, iterationsPerPixel);
		const double end = omp_get_wtime();
		const double dif = end - begin;
		if (dif < minTime)
		{
			minTime = dif;
		}
	}
	delete[] iterationsPerPixel;
	printf("%dx%d with %d iterations: %fs\r\n", w, h, maxIterations, minTime);
}

void TestSpeedWith(INT w, INT h, INT maxIterations, INT testsCount)
{
	printf("Sequential ");
	PrintAbsoluteSpeed(w, h, maxIterations, testsCount, MandelbrotSetIterationsSequential);

	printf("Seq optim  ");
	PrintAbsoluteSpeed(w, h, maxIterations, testsCount, MandelbrotSetIterationsSequentialOptimized);
}

void TestSpeed()
{
	const INT testsCount = 5;

	TestSpeedWith(512, 512, 256, testsCount);
	TestSpeedWith(512, 512, 512, testsCount);
	TestSpeedWith(512, 512, 1024, testsCount);

	TestSpeedWith(1024, 1024, 256, testsCount);
	TestSpeedWith(1024, 1024, 512, testsCount);
	TestSpeedWith(1024, 1024, 1024, testsCount);

	TestSpeedWith(2048, 2048, 256, testsCount);
	TestSpeedWith(2048, 2048, 512, testsCount);
	TestSpeedWith(2048, 2048, 1024, testsCount);
	
}