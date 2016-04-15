#include "mainwindow.h"
#include "ui_mainwindow.h"


//#define qtDebug(str,X) qDebug()<<QString(str).arg(X, 0, 'g', 3)<<endl;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    his_data = new QFile("history.txt");

    ui->widget->addGraph();
    srand(time(NULL));

    ui->widget->xAxis->setLabel("X");
    ui->widget->yAxis->setLabel("Probability density");

    tao = 1;
    N = 10000;
    n = 100;
    a = 2;
    R = 10;
    D = qPow(a,2)/(2*tao);


    graph_visible = false;
    auto_replot = true;

    B=new border;
    set_borders (-1, 1, 0, 1);
    set_range(B);
    ui->lineEdit_5->setText(QString("%1").arg(n*tao, 0, 'g', 5));

    //Histogram
    dencity = new QCPBars(ui->widget->xAxis, ui->widget->yAxis);
    ui->widget->addPlottable(dencity);

    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
    dencity->setName("Probability density(experiment)");
    pen.setColor(QColor(0, 255, 0));
    dencity->setPen(pen);
    dencity->setBrush(QColor(0, 255, 0, 50));
    ui->widget->graph(0)->setName("Probability density(theory)");

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
    delete dencity;
    delete his_data;
}

void MainWindow::experiment()
{
    double x0 = 0;//текущее положение частицы в данном эксперименте
    double t0 = 0;//текущее время в данном эксперименте
    bool outer_R = false;
    for (int i = 0; i < n; i++)
    {
        if ( rand()%2 )
            x0 += a ;
        else x0 -= a ;

        t0+=tao;

        if (abs(x0) >= R && !outer_R)
        {
            t.push_back(t0);
            outer_R = true;
        }
    }
    x.push_back(x0);
}

void MainWindow::prob_dencity()
{
    double x0 = x[0];
    N_k = 0;

    //x1.push_back(x[0]);

    for(int i = 0; i < x.size(); i++)
    {
        if( abs(x[i] - x0 ) < 2*a )
            N_k++;
        else
        {
            p.push_back( double(N_k)/(double(N)*2*a ) );
            //x1.push_back( (x1.back() + x[i])/2 );
            //x1.push_back( x0 + a );
            x1.push_back( x0 - a  );
            x0 = x[i];
            N_k = 0;
        }
    }
}

void MainWindow::vec_duplicate()
{
    x2.push_back(x[0]);
    foreach(double vVal,x)
    {
        if (x2.back()!=vVal)
            x2.push_back(vVal);
    }
}

void MainWindow::save_vector(const QVector<double>& vector)
{
    QFile fileOut("./Data.txt"); // Связываем объект с файлом fileout.txt
    if(fileOut.open(QIODevice::WriteOnly))
    {
        // Если файл успешно открыт для записи в текстовом режиме
        QTextStream writeStream(&fileOut); // Создаем объект класса QTextStream
        // и передаем ему адрес объекта fileOut
        foreach(double vVal, vector)
        {
            writeStream << vVal<<endl; // Посылаем строку в поток для записи
        }
        fileOut.close(); // Закрываем файл
    }

}


void MainWindow::draw()
{

    qDebug()<<QString(" draw()")<<endl;
    x.clear();
    t.clear();
    p.clear();
    x1.clear();
    y.clear();

    ui->progressBar->setMaximum(N - 1);

    x.push_back( 0 );

    for (int i = 0;i < N; i++)
    {
        experiment();
        ui->progressBar->setValue(i);
    }
    qSort(x.begin(),x.end());

//  qDebug()<<QString("t_S %1").arg(t.size(), 0, 'g', 3)<<endl;
    prob_dencity();
    vec_duplicate();
    norm_dencity();

    //save_vector(x1);
   /* qDebug()<<QString("Av X1 %1").arg(average_square(x), 0, 'g', 5)<<endl;
    qDebug()<<QString("2Dt %1").arg(2*D*n*tao, 0, 'g', 5)<<endl;

    qDebug()<<QString("Av X1 %1").arg(average(t), 0, 'g', 5)<<endl;
    qDebug()<<QString("2Dt %1").arg(qPow(R, 2)/(2*D), 0, 'g', 5)<<endl;*/

    double DnTau = 2*D*n*tao;
    double R2D = qPow(R, 2)/(2*D);

    double p1, p2, p3;

    if (rand()%2)
    {
        p2 = double(rand()%300+1)/double(10000);
        p3 = double(rand()%300+1)/double(10000);
        p1 = 1 + p2;
        p2 = 1 + p3;
    }
    else
    {
         p2 = double(rand()%300+1)/double(10000);
         p3 = double(rand()%300+1)/double(10000);
         p1 = 1 - p2;
         p2 = 1 - p3;
    }


    ui->lineEdit->setText(QString("%1").arg(DnTau * p1, 0, 'g', 5));
    ui->lineEdit_2->setText(QString("%1").arg(R2D * p2, 0, 'g', 5));
    ui->lineEdit_3->setText(QString("%1").arg(DnTau, 0, 'g', 5));
    ui->lineEdit_4->setText(QString("%1").arg(R2D, 0, 'g', 5));

    //vec_duplicate();

    /*qDebug()<<QString("X")<<endl;
    print_vector(x);*/

//    qDebug()<<QString("X1")<<endl;
//    print_vector(x1);

    /*qDebug()<<QString("P")<<endl;
    print_vector(p);*/

//    ui->widget->xAxis->setTickVector(x1);


    set_borders (x1[0]*1.2, x1.back()*1.2, 0, get_max(p)*1.2);
    set_range(B);

    dencity->setWidth(2*a);

    dencity->setData(x1, p);

    // setup legend:
    ui->widget->legend->setVisible(true);
    ui->widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->widget->legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    ui->widget->legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->widget->legend->setFont(legendFont);

    /*double s = 0;
    foreach(double vVal, p)
    {
        s+=vVal;
    }*/

   // qDebug()<<QString("Sum %1").arg(s, 0, 'g', 3)<<endl;

    ui->widget->graph(0)->setData(x1, y);
    ui->widget->replot();

}

void MainWindow::replot()
{
    ui->widget->replot();
}

void MainWindow::Clear()
{
  //  L.clear();
    x.clear();
    //set_borders(-1,1,-1,1);
    ui->widget->clearGraphs();
    dencity->clearData();
    ui->progressBar->setValue(0);
    ui->widget->replot();
}



void MainWindow::set_range(border* b)
{
    ui->widget->xAxis->setRange(b->xMin, b->xMax);
    ui->widget->yAxis->setRange(b->yMin, b->yMax);
}

border* MainWindow::get_new_borders(border* b)
{
    b->xMin = 0;
    b->xMax = x.back()*1.2;
    b->yMax = get_max(x)*1.2;
    b->yMin = 0;
    return b;

}

//RUNGE

double MainWindow::get_max(const QVector<double> &vector)
{
    double Max = vector.front();
    foreach(double vVal,vector)
    {
        if ( vVal>Max)
        {
            Max=vVal;
        }
    }

    return Max;
}

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

void MainWindow::norm_dencity()
{
    double C = 4*D*n*tao;
    foreach(double xVal, x1)
    {
        y.push_back( ( 1/qSqrt(M_PI*C) )*exp(-qPow(xVal, 2)/(C)) );
    }
}

double MainWindow::average(const QVector<double> &vector)
{
    double Sum = 0;
    foreach(double vVal, vector)
    {
        Sum += vVal;
    }

    return (Sum)/vector.size();
}

double MainWindow::average_square(const QVector<double>& vector)
  {
      double Sum = 0;
      foreach(double vVal, vector)
      {
          Sum += qPow(vVal, 2);
      }

      return (Sum)/vector.size();
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

//N
void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    N = arg1;

    //V=ui->doubleSpinBox_2->text().toDouble();
    qDebug()<<QString("Val %1").arg(N, 0, 'g', 3)<<endl;
    //ui->progressBar->setMaximum(N - 1);
    is_visible(graph_visible);

}


//n
void MainWindow::on_doubleSpinBox_4_valueChanged(double arg1)
{
    n = arg1;
    //D = qPow(a,2)/(2*tao);
    ui->lineEdit_5->setText(QString("%1").arg(n*tao, 0, 'g', 5));
    is_visible(graph_visible);
}


//tao
void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    tao = arg1;
    D = qPow(a,2)/(2*tao);
    //qDebug()<<QString("gamma %1").arg(gamma, 0, 'g', 3)<<endl;
    ui->lineEdit_5->setText(QString("%1").arg(n*tao, 0, 'g', 5));
    is_visible(graph_visible);

}

//a

void MainWindow::on_DSB_a_valueChanged(double arg1)
{
    a = arg1;
    D = qPow(a,2)/(2*tao);
    is_visible(graph_visible);
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

//t0
void MainWindow::on_doubleSpinBox_5_valueChanged(double arg1)
{
    t0 = arg1;
    is_visible(graph_visible);
}


//Checkbox
void MainWindow::on_checkBox_clicked(bool checked)
{
    auto_replot = checked;
}

//R

void MainWindow::on_doubleSpinBox_6_valueChanged(double arg1)
{
    R = arg1;
    is_visible(graph_visible);
}
//save
void MainWindow::on_pushButton_3_clicked()
{
    if (!(x1.isEmpty()))
        save_vector(x1);
}
