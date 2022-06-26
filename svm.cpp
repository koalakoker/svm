#include "svm.h"
#include <QDebug>

#define SQRT3 1.73205f

SVM::SVM()
{

}

int sign(float x) {
    if (x >= 0) return 1;
    return 0;
}

int N_Sector_Lookup[] = {2, 6, 1, 4, 3, 5};

int SVM::sector(AlphaBeta v) {
    float vref1 = v.beta;
    float vref2 = (SQRT3 / 2) * v.alpha - (1.0f/2.0f) * v.beta;
    float vref3 = -(SQRT3 / 2) * v.alpha - (1.0f/2.0f) * v.beta;

    int N = sign(vref1) + (2 * sign(vref2)) + (4 * sign(vref3));

    return N_Sector_Lookup[N-1];
}

Duty SVM::calculate(AlphaBeta v) {
    Duty duty;

    qDebug() << "Sector:" << SVM::sector(v);


    return duty;
}
