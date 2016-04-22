#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //his_data = new QFile("history.txt");

    ui->widget->addGraph(); //A
    ui->widget->addGraph(); //A_v
    ui->widget->addGraph(); //f
    ui->widget->addGraph(); //f_v

    ui->widget->graph(0)->setName("A");
    ui->widget->graph(1)->setName("A (Hilbert transform)");
    ui->widget->graph(2)->setName("f");
    ui->widget->graph(3)->setName("f (Hilbert transform)");

    ui->widget->graph(0)->setPen(QPen(Qt::blue));
    ui->widget->graph(1)->setPen(QPen(Qt::red));
    ui->widget->graph(2)->setPen(QPen(Qt::green));
    ui->widget->graph(3)->setPen(QPen(Qt::black));

    srand(time(NULL));

    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");


    //(1)settinng the paprametres of source signal
    A = 1;
    a = 4;
    alpha = 0.2;
    f0 = 2;

    //(2)settinng the uniform grid
    N = 2048;
    L = 16;




    graph_visible = false;
    auto_replot = true;

    B=new border;
    set_borders (-1, 1, 0, 1);
    set_range(B);


    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

}

void print_vector(const QVector<double>& vector)
{
    //foreach (double val, vector)
    for (int i = 0; i < vector.size(); i++)
        qDebug()<<QString("Val %1  ").arg(vector[i], 0, 'g', 3);
}


MainWindow::~MainWindow()
{
    delete B;
    delete ui;
    //delete his_data;
}


void MainWindow::save_vector(const QVector<double>& vector1, const QVector<double>& vector2)
{
    QFile fileOut("./Data.txt"); // Связываем объект с файлом fileout.txt
    if(fileOut.open(QIODevice::WriteOnly))
    {
        // Если файл успешно открыт для записи в текстовом режиме
        QTextStream writeStream(&fileOut); // Создаем объект класса QTextStream
        // и передаем ему адрес объекта fileOut
        for (int i = 0; i < N - 1 ;i++)
            writeStream << i<<"\t"<<vector1[i]<<"\t"<<vector2[i]<<endl; // Посылаем строку в поток для записи

        fileOut.close(); // Закрываем файл
    }

}


void MainWindow::digitizing()
{
    u.clear();
    W.clear();
    A_or.clear();
    f.clear();
    x.clear();
    F_or.clear();

    u.reserve(N);
    W.reserve(N);
    A_or.reserve(N);
    f.reserve (N);
    x.reserve(N);
    F_or.reserve(N);

    for (int j = 0; j < N; j++ )
    {
        x.push_back(  double(-L/2) + double(j)*double(L)/double(N) );
        F_or.push_back( F(alpha, a, f0, x.back())  );
        u.push_back( A*cos ( F_or.back() ) );
        A_or.push_back( A );
        f.push_back( f0*( 1 - alpha*cos( M_PI*x.back()/a )  ) );
    }
}

double MainWindow::F(double alpha, double a, double f0, double x )
{
    return 2*M_PI*f0*x - 2 * alpha * a * f0 * sin(x * M_PI / a);
}

void MainWindow::hilbert_transform()
{
    //прямое преобразование
    // u сформирован
    W_v.clear();
    A_v.clear();
    f_v.clear();
    F_v.clear();
    u_v.clear();

    W_v.reserve(N);
    A_v.reserve(N);
    f_v.reserve(N);
    F_v.reserve(N);
    u_v.reserve(N);

//    W = dft(convert(u));

    W = fft(convert(u));


    for(int i = 0; i < N; i++)
    {
        W_v.push_back( W[i]*complex<double>(-1, 0)*complex<double>(0, 1)*complex<double>(s_k(i, N), 0) );
    }
    //TODO
    u_v = convert (ifft(W_v));

    A_v.push_back( sqrt( u[0]*u[0] + u_v[0]*u_v[0] ));
    F_v.push_back( atan2( u_v[0], u[0] ) );

    for(int i = 1; i < N; i++)
    {
       A_v.push_back( sqrt( u[i]*u[i] + u_v[i]*u_v[i] ));
       F_v.push_back( atan2( u_v[i], u[i] ) );
    }

    for (int i = 1; i < N; i++) {
        if(F_v[i] < F_v[i-1] )
        {
            for (int j = i; j < N; j++) {
                F_v[j] += 2*M_PI;
            }
        }
    }

    for(int i = 0; i < N - 1; i++)
    {
        f_v.push_back( (F_v[i + 1] - F_v[i])/((x[i+1] - x[i])*2*M_PI) );
    }

}

double MainWindow::standard_deviation(const QVector<double>& vector1, const QVector<double>& vector2)
{
    double Sum = 0;

    for(int i = 0; i < std::min(vector1.size(), vector2.size()); i++)
        Sum += qPow(vector1[i] -vector2[i]  ,2);
    double res = qSqrt(Sum/vector1.size());
    return res;
}
int MainWindow::s_k (int k, int N)
{
    if(k == 0)
        return 0;
    if( k>= N/2)
        return -1;
    if (k < N/2)
        return 1;

}

//###Draw
void MainWindow::draw()
{
    digitizing();
    hilbert_transform();
    ui->lineEdit->setText(QString("%1").arg(standard_deviation(A_or, A_v), 0, 'g', 5));
    ui->lineEdit_2->setText(QString("%1").arg(standard_deviation(f, f_v), 0, 'g', 5));

    set_borders (x[0], x.back()*1.2, *std::min_element(f_v.begin(), f_v.end())*1.2, *std::max_element(f_v.begin(), f_v.end())*1.2);
    set_range(B);

    ui->widget->legend->setVisible(true);
    ui->widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->widget->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->widget->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->widget->legend->setFont(legendFont);

    ui->widget->graph(0)->setData(x, A_or);
    ui->widget->graph(1)->setData(x, A_v);
    ui->widget->graph(2)->setData(x, f);
    ui->widget->graph(3)->setData(x, f_v);
    ui->widget->replot();

}

void MainWindow::replot()
{
    ui->widget->replot();
}

void MainWindow::Clear()
{
    ui->widget->clearGraphs();
    ui->widget->replot();
}



void MainWindow::set_range(border* b)
{
    ui->widget->xAxis->setRange(b->xMin, b->xMax);
    ui->widget->yAxis->setRange(b->yMin, b->yMax);
}


//RUNGE


void MainWindow::set_borders(double xMin, double xMax, double yMin, double yMax)
{

          B->xMin = xMin;
          B->xMax = xMax;
          B->yMin = yMin;
          B->yMax = yMax;
 }

void MainWindow::is_visible (bool gv)
{
    if(gv && auto_replot) draw();
}


//##############################
//slots
//##############################

void MainWindow::on_pushButton_clicked()
{
    graph_visible = true;
    //ui->widget->addGraph();
    draw();
}

//clear
void MainWindow::on_pushButton_2_clicked()
{
    graph_visible =false;
    qDebug()<<QString("clear")<<endl;
    Clear();
    ui->widget->addGraph();
    //убрать график
}


//Checkbox
void MainWindow::on_checkBox_clicked(bool checked)
{
    auto_replot = checked;
}

//save
void MainWindow::on_pushButton_3_clicked()
{
    if (!(x.isEmpty()) && !(f_v.isEmpty()))
        save_vector(x, f_v);
}
