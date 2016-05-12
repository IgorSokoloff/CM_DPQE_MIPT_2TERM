#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <math.h>
#include <stdlib.h>
#include "qcustomplot.h"
#include <QFile>
#include <algorithm>
#include <complex>

#include "FFTA/utilities.h"
#include "FFTA/ffta.h"


namespace Ui {
class MainWindow;
}

//todo: структура с данными о X, Y Min, Max чтобы изменит масштаб графика
//get_struct
//for_each для прохода по данным

typedef struct
{
    double xMin;
    double xMax;
    double yMin;
    double yMax;
}border;

typedef struct{
    double vMin;
    double tettaMin;
}Data;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void draw();
    void replot();
    void set_range(border* b);
    void set_borders (double xMin, double xMax, double yMin, double yMax);

    void save_vector(const QVector<double>& vector1, const QVector<double>& vector2);
    void is_visible (bool gv);
    void Clear();

    //Elkin_36
    void digitizing();
    void hilbert_transform();
    double standard_deviation(const QVector<double>& vector1, const QVector<double>& vector2);
    int s_k (int k, int N);
    double A (double x, double a);
    double F (double alpha, double a, double f0, double x);

    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_clicked(bool checked);

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;


    vector<double> u;
    vector<complex<double>> W;
    vector<complex<double>> W_v; //W + vave (after Hilbert transform)


    vector<double> u_v;           //u + vave ()

    //double A;
   // QVector<double> F_or;
    QVector<double> F_v;
    QVector<double> x;
    QVector<double> A_or;   //A_or - A original
    QVector<double> A_v;


    QVector<double> f_v;
    QVector<double> f;

    double a, alpha, f0;
    int N, L;


    border* B;
    bool graph_visible;
    bool auto_replot;

};

#endif // MAINWINDOW_H
