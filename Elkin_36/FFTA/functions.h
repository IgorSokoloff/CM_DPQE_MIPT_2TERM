#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <cmath>

double Amplitude(double x)
{
	return 2 * cos(x);
}

double Phase(double x)
{
	return 2 * x;
}

double f(double x)
{
	return Amplitude(x) * cos(Phase(x));
}

#endif // FUNCTIONS_H
