#include "ffta.h"
using std::swap;
vector<complex<double> > ffta (vector<complex<double> > a, bool invert)
{
	int N = (int) a.size();

	for (int i = 1, j = 0; i < N; ++i) {
		int bit = N >> 1;
		for (; j >= bit; bit >>= 1)
			j -= bit;
		j += bit;
		if (i < j)
			swap(a[i], a[j]);
	}

	for (int len = 2; len <= N; len <<= 1) {
        double ang = 2 * M_PI / len * (invert ? 1 : -1);
		complex<double> wlen (cos(ang), sin(ang));
		for (int i = 0; i < N; i += len) {
			complex<double> w (1);
			for (int j = 0; j < len / 2; ++j) {
				complex<double> u = a[i + j];
				complex<double> v = a[i + j + len / 2] * w;
				a[i + j] = u + v;
				a[i + j + len / 2] = u - v;
				w *= wlen;
			}
		}
	}
	if (invert)
		for (int i = 0; i < N; ++i)
			a[i] /= N;

	return a;
}

vector<complex<double> > fft(vector<complex<double> > a)
{
	return ffta(a, false);
}

vector<complex<double> > ifft(vector<complex<double> > a)
{
	return ffta(a, true);
}

vector<complex<double> > dft(vector<complex<double> > a)
{
	int N = a.size();
	vector<complex<double> > W(N, 0.0);

	for (int k = 0; k < N; k++) {
		double ang = - 2 * M_PI * k / N;
		for (int n = 0; n < N; n++) {
			W[k] += a[n] * complex<double>(cos(ang * n), sin(ang * n));
		}
	}
	return W;
}

vector<complex<double> > idft(vector<complex<double> > a)
{
	int N = a.size();
	vector<complex<double> > W(N, 0.0);

	for (int k = 0; k < N; k++) {
		double ang = 2 * M_PI * k / N;
		for (int n = 0; n < N; n++) {
			W[k] += a[n] * complex<double>(cos(ang * n), sin(ang * n));
		}
        W[k] /= N;
	}
    return W;
}
