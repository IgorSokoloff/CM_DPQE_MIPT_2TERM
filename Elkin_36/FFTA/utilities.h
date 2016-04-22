#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <complex>
#include <functional>
#include <iostream>

using std::vector;
using std::complex;
using std::function;

/**
 * @brief Перегруженная функция, преобразующая vector<double> в
 * vector<complex<double> > и обратно.
 * @param v
 * @return Преобразованный вектор
 */
vector<complex<double> > convert(const vector<double> &v);

/**
 * @brief Перегруженная функция
 * @param v
 * @return Преобразованный вектор
 */
vector<double> convert(const vector<complex<double> > &v);

/**
 * @brief Возвращает вектор из модулей
 * @param Комплексный вектор
 * @return Вектор из модулей
 */
vector<double> vectorAbs(const vector<complex<double> > &v);

/**
 * @brief linear Создаёт линейно приближенный отрезок
 * @param a Начало отрезка
 * @param b Конец Отрезка
 * @param N Число точек
 * @return Отрезок
 */
vector<double> linear(double a, double b, int N);

/**
 * @brief Создаёт вектор из функции и координатного вектора
 * @param f Функция
 * @param x Входной вектор
 * @return Результат применения функции к вектору
 */
vector<double> fromFunction(function<double(double)> f, vector<double> x);
#endif // UTILITIES_H
