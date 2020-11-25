#include <Windows.h>
#include "MandelbrotSetCalculation.h"
#include <math.h>

#define MANDELBROT_LEFT -2.25
#define MANDELBROT_RIGHT 0.75
#define MANDELBROT_TOP -1.5
#define MANDELBROT_BOTTOM 1.5

void MandelbrotSetIterationsSequential(INT w, INT h, INT maxIterations, INT* iterationsPerPoint)
{
	const double xStep = (MANDELBROT_RIGHT - MANDELBROT_LEFT) / w;
	const double yStep = (MANDELBROT_BOTTOM - MANDELBROT_TOP) / h;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			const double pointX = MANDELBROT_LEFT + x * xStep;
			const double pointY = MANDELBROT_TOP + y * yStep;

			double zX = pointX;
			double zY = pointY;
			int iterations = 0;

			for (; zX * zX + zY * zY < 4 && iterations < maxIterations; iterations++)
			{
				// z0 = c = x + iy
				// z1 = z0 * z0 + c = (x + iy)(x + iy) + c =
				// = x^2 + 2xyi + y^2i^2 + c =
				// = (x^2 - y^2) + (2xy)i + c

				double zXSquare = zX * zX - zY * zY;
				double zYSquare = 2 * zX * zY;

				zX = zXSquare + pointX;
				zY = zYSquare + pointY;
			}
			iterationsPerPoint[x + y * w] = iterations;
		}
	}
}

inline bool doesBelongToMainCardioid(double x, double y)
{
	const double pointPolarAngle = atan2(y, x - 0.25);
	const double cardioidRadiusAtThisAngle = 0.5 - 0.5 * cos(pointPolarAngle);
	const double pointRadiusAtThisAngle = sqrt((x - 0.25) * (x - 0.25) + y * y);
	return pointRadiusAtThisAngle <= cardioidRadiusAtThisAngle;
}

void MandelbrotSetIterationsSequentialOptimized(
	INT w, INT h, INT maxIterations, INT* iterationsPerPoint)
{
	const double xStep = (MANDELBROT_RIGHT - MANDELBROT_LEFT) / w;
	const double yStep = (MANDELBROT_BOTTOM - MANDELBROT_TOP) / h;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			const double pointX = MANDELBROT_LEFT + x * xStep;
			const double pointY = MANDELBROT_TOP + y * yStep;

			int iterations = 0;

			if (doesBelongToMainCardioid(pointX, pointY))
			{
				iterations = maxIterations;
			}
			else
			{
				double zX = pointX;
				double zY = pointY;

				for (; zX * zX + zY * zY < 4 && iterations < maxIterations; iterations++)
				{
					double zXSquare = zX * zX - zY * zY;
					double zYSquare = 2 * zX * zY;

					zX = zXSquare + pointX;
					zY = zYSquare + pointY;
				}
			}
			iterationsPerPoint[x + y * w] = iterations;
		}
	}
}