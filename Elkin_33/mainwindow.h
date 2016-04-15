#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <math.h>
#include <stdlib.h>
#include "qcustomplot.h"
#include <QFile>
#include <algorithm>
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
    border* get_new_borders(border* b);
    void set_borders (double xMin, double xMax, double yMin, double yMax);

    double get_max(const QVector<double>& vector);

    void set_characteristicks();
    void experiment(int k);

    void prob();
    void norm_dencity();


    double average(const QVector<double>& vector);
    double average_square(const QVector<double>& vector);

    void vec_duplicate();

    void save_vector(const QVector<double>& vector);

    int get_it_min();


    void is_visible (bool gv);
    void Clear();

    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_pushButton_2_clicked();

    void on_checkBox_clicked(bool checked);

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    QVector<int> ch;//характеристика
    QVector<int> n;
    QVector<double> p;
    QVector<double> k;

    QFile* his_data;

    border* B;

    //N - претенденты
    //K - испытания
    //n(вектор) - номера претендента
    int N, K;
    int ch_k; //максимальная оценка первых k претендентов
    int ch_N_k; //максимальная оценка последующих N - k претендентов
    int ch_N; //максимальная оценка  N претендентов

    bool graph_visible;
    bool auto_replot;

};

#endif // MAINWINDOW_H
