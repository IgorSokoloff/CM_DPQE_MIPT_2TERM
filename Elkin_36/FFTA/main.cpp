/**
  *@file В этом файле мы сравниваем быстродействие обычного
  * и быстрго преобразований Фурье
  */
#include <QApplication>

#include <iostream>
#include <chrono>
#include <qcustomplot.h>

#include "ffta.h"
#include "functions.h"
#include "utilities.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char ** argv)
{
	int N;
	cout << "Input size (Note: size must be pow of two)" << endl;
	cin >> N;

	vector<double> x = linear(-10000, 10000, N);
	vector<double> y = fromFunction(f, x);

	high_resolution_clock::time_point start = high_resolution_clock::now();
	y = vectorAbs((fft(convert(y))));

	duration<double> d = high_resolution_clock::now() - start;

	clog << "It takes :" << d.count() << 's' << endl;
	///Вставим в вектор амплитуды, соответствующие отрицательным частотам
	x = linear(-2 * M_PI, 2 * M_PI, 2 * N);
	y.insert(y.end(), y.rbegin(), y.rend());

	QApplication app(argc, argv);
	///График
	QCustomPlot plot;
	plot.resize(500, 500);
	plot.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	plot.addGraph();
	plot.graph(0)->setData(QVector<double>::fromStdVector(x),
						   QVector<double>::fromStdVector(y));

	plot.xAxis->setRange(*min_element(x.begin(), x.end()),
						 *max_element(x.begin(), x.end()));

	plot.yAxis->setRange(*min_element(y.begin(), y.end()),
						 *max_element(y.begin(), y.end()));

	plot.show();

	return app.exec();
}
