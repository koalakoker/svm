#include "mainwindow.h"
#include "mwplot.h"
#include "svm.h"
#include "alphabeta.h"
#include <math.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();


    MWPlot* mwplot = new MWPlot("Test");

    float m = 0.5f;
    int nSect = 6;
    int angleEnd = 60 * nSect;
    AlphaBeta v;
    for (int i = 0; i < angleEnd; i+=1) {
        float th = (float)(i) * M_PI/180;
        v.alpha = m * cos(th);
        v.beta = m * sin(th);
        Duty d = SVM::calculate(v);
        //qDebug() << i << d.a << d.b << d.c;
        mwplot->addPoint(i,SData(d.a, d.b, d.c));
    }

    mwplot->show();
    mwplot->updatePlot();

    mwplot->exportData("svm.txt");

    return a.exec();
}
