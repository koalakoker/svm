#include "mainwindow.h"
#include "svm.h"
#include "alphabeta.h"
#include <math.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();

    float m = 1.0f;
    int nSect = 2;
    int angleEnd = 60 * nSect;
    AlphaBeta v;
    for (int i = 0; i < angleEnd; i+=1) {
        float th = (float)(i) * M_PI/180;
        v.alpha = m * cos(th);
        v.beta = m * sin(th);
        Duty d = SVM::calculate(v);
        qDebug() << i << d.a << d.b << d.c;
    }
}
