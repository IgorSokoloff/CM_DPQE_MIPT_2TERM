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

    ui->widget->xAxis->setLabel("k");
    ui->widget->yAxis->setLabel("Pk");

    K = 30000;
    N = 100;
    ch_k = -1;

    graph_visible = false;
    auto_replot = true;

    B=new border;
    set_borders (-1, 1, 0, 1);
    set_range(B);

    ui->widget->graph(0)->setName("Probability");
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
    delete his_data;
}

void MainWindow::set_characteristicks()
{
    for (int i = 0; i < N; i++)
    {
        n.push_back(rand());
    }
}

void MainWindow::experiment(int k)
{
    for(int i = k; i < N;i++)
    {
        if(ch[k] > ch_k)
        {

        }
    }

    n.push_back( N );
}


void MainWindow::prob()
{
    int n0 = n[0];
    int N_k = 0;

    QVector<int> number_of_candidates;


    //x1.push_back(x[0]);

    for(int i = 0; i < n.size() ; i++)
    {
        if( n[i] == n0 )
            N_k++;
        else
        {
            number_of_candidates.push_back(N_k);
            N_k = 1;
            n0 = n[i];
        }
    }
    N_k = *std::max_element(number_of_candidates.begin(), number_of_candidates.end());    
    p.push_back( double(N_k)/(double(K) ) );
        N_k = 0;

}


void MainWindow::save_vector(const QVector<double>& vector)
{
    QFile fileOut("./Data.txt"); // Связываем объект с файлом fileout.txt
    if(fileOut.open(QIODevice::WriteOnly))
    {
        // Если файл успешно открыт для записи в текстовом режиме
        QTextStream writeStream(&fileOut); // Создаем объект класса QTextStream
        // и передаем ему адрес объекта fileOut
        for (int i = 0; i < vector.size() ;i++)
            writeStream << i<<" "<<vector[i]<<endl; // Посылаем строку в поток для записи

        fileOut.close(); // Закрываем файл
    }

}

//###Draw
void MainWindow::draw()
{

    //qDebug()<<QString(" draw()")<<endl;
    //n.clear();
    p.clear();
    k.clear();

    p.reserve(N);
    k.reserve(N);

    if (!ch.isEmpty())
    {
        ch.clear(); 
    }

    k.push_back (0);
    p.push_back(1/N);

    int i = 1;
    for ( i = 1; i < N; i++)//считаем что на первой итерации уже один кандидат отвергнут
    {
        int N_k  = 0;
        for (int j = 0; j < K; j++)
        {
            //QVector<int>::iterator it = ch.begin();
            ch.reserve(N);

            //qDebug()<<QString("ch.size").arg(ch.size(), 0, 'g', 3)<<endl;

            for (int j2 = 0; j2 < i; j2++)//даем оценку первым k кандидатам
            {
                ch.push_back( rand()%32767 );
            }
           // it++;
            if (!ch.isEmpty())
                ch_k = *std::max_element(ch.begin(), ch.end());


            for (int j3 = i; j3 < N; j3++)//даем оценку последующим N - k кандидатам
            {
                ch.push_back( rand()%32767 );
            }

            if (!ch.isEmpty())
                ch_N = *std::max_element(ch.begin(), ch.end());

            //здесь необходимо потестить
            for(int j4 = i; j4 < N; j4++)
            {
                if(ch[j4] > ch_k)
                {
                    if (ch[j4] == ch_N )
                        N_k++;
                    else break;
                }

            }

            ch.clear();
            ui->progressBar_2->setValue(j);
        }

        k.push_back(double(i));
        p.push_back(double(N_k)/double(K));
        ui->progressBar->setValue(i);
    }

    p.push_back(double(1)/double(N));


    set_borders (k[0], k.back()*1.2, 0, get_max(p)*1.2);
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


    ui->widget->graph(0)->setData(k, p);
    ui->widget->replot();

}

void MainWindow::replot()
{
    ui->widget->replot();
}

void MainWindow::Clear()
{
  //  L.clear();
    p.clear();
    k.clear();
    //set_borders(-1,1,-1,1);
    ui->widget->clearGraphs();
    ui->progressBar->setValue(0);
    ui->progressBar_2->setValue(0);
    ui->widget->replot();
}



void MainWindow::set_range(border* b)
{
    ui->widget->xAxis->setRange(b->xMin, b->xMax);
    ui->widget->yAxis->setRange(b->yMin, b->yMax);
}
/*
border* MainWindow::get_new_borders(border* b)
{
    b->xMin = 0;
    b->xMax = x.back()*1.2;
    b->yMax = get_max(x)*1.2;
    b->yMin = 0;
    return b;

}*/

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

//K
void MainWindow::on_doubleSpinBox_2_valueChanged(double arg1)
{
    K = arg1;
    //V=ui->doubleSpinBox_2->text().toDouble();
    qDebug()<<QString("Val %1").arg(K, 0, 'g', 3)<<endl;
    ui->progressBar->setMaximum(K - 1);
    is_visible(graph_visible);

}



//N
void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    N = arg1;
    //qDebug()<<QString("gamma %1").arg(gamma, 0, 'g', 3)<<endl;
    ui->progressBar->setMaximum(N);
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


//Checkbox
void MainWindow::on_checkBox_clicked(bool checked)
{
    auto_replot = checked;
}

//R

//save
void MainWindow::on_pushButton_3_clicked()
{
    if (!(p.isEmpty()))
        save_vector(p);
}
