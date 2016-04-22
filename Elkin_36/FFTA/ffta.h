#ifndef FFTA_H
#define FFTA_H

#include <vector>
#include <complex>

using std::vector;
using std::complex;

/**
 * @brief Функция, реализующая быстрое преобразование Фурье (Fast Fourier Transform).
 * @param a Комплексный вектор
 * @param invert Значение типа bool, указывающее какое преобразование выполнять:
 * прямое(false), обратное(true)
 * @return Преобразованный вектор
 */
vector<complex<double> > ffta (vector<complex<double> > a, bool invert);

/**
 * @brief Функция, реализующая прямое быстрое преобразование Фурье.
 * @param a Комплексный вектор
 * @return Преобразованный вектор
 */
vector<complex<double> > fft (vector<complex<double> > a);

/**
 * @brief Функция, реализующая обратное быстрое преобразование Фурье.
 * @param a Комплексный вектор
 * @return Преобразованный вектор
 */
vector<complex<double> > ifft (vector<complex<double> > a);

/**
 * @brief dft Функция реализующая прямое преобразование фурье "в лоб".
 * @param a Комплексный вектор
 * @return Преобразованный вектор
 */
vector<complex<double> > dft(vector<complex<double> > a);

/**
 * @brief dft Функция реализующая обратное преобразование фурье "в лоб".
 * @param a Комплексный вектор
 * @return Преобразованный вектор
 */
vector<complex<double> > idft(vector<complex<double> > a);
#endif // FFTA_H

