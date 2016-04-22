#include "utilities.h"
vector<complex<double> > convert(const vector<double> &v)
{
	size_t N = v.size();
	vector<complex<double> >vcd(N);
	for (size_t i = 0; i < N; ++i) {
		vcd[i] = complex<double>(v[i]);
	}
	return vcd;
}

vector<double> convert(const vector<complex<double> > &v)
{
	size_t N = v.size();
	vector<double>vcd(N);
	for (size_t i = 0; i < N; ++i) {
		vcd[i] = v[i].real();
	}
	return vcd;
}

vector<double> vectorAbs(const vector<complex<double> > &v)
{
	size_t N = v.size();
	vector<double>vcd(N);
	for (size_t i = 0; i < N; ++i) {
		vcd[i] = abs(v[i]);
	}
	return vcd;
}

vector<double> linear(double a, double b, int N)
{
	vector<double> v(N);
	double delta = (b - a) / N;
	for (int i = 0; i < N; ++i) {
		v[i] = a + delta * i;
	}
	return v;
}

vector<double> fromFunction(function<double (double)> f, vector<double> x)
{
	int N = x.size();
	vector<double> v(N);
	for (int i = 0; i < N; ++i) {
		v[i] = f(x[i]);
	}
	return v;
}
