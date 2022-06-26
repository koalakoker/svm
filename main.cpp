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

    float m = 0.5f;
    AlphaBeta v;
    for (int i = 0; i < 60; i+=1) {
        float th = (float)(i) * M_PI/180;
        v.alpha = m * cos(th);
        v.beta = m * sin(th);
        qDebug() << i << SVM::calculate(v).toString();
    }
}
