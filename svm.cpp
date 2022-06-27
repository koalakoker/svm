#include "svm.h"
#include <QDebug>

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
    Duty d;

    int sector = SVM::sector(v);

    switch (sector) {
    case 1:
    {
        float t1 = v.alpha - (v.beta / SQRT3);
        float t2 = v.beta * (2.0f / SQRT3);
        d.c = (1 - t1 - t2) / 2.0f;
        d.b = d.c + t2;
        d.a = d.b + t1;
        break;
    }
    case 2:
    {
        float t2 = v.alpha + (v.beta / SQRT3);
        float t3 = - v.alpha + (v.beta / SQRT3);
        d.c = (1 - t2 - t3) / 2.0f;
        d.a = d.c + t2;
        d.b = d.a + t3;
        break;
    }
    case 3:
    {
        float t3 = v.beta * (2.0f / SQRT3);
        float t4 = - v.alpha - (v.beta / SQRT3);
        d.a = (1 - t3 - t4) / 2.0f;
        d.c = d.a + t4;
        d.b = d.c + t3;
        break;
    }
    case 4:
    {
        float t4 = - v.alpha + (v.beta / SQRT3);;
        float t5 = - v.beta * (2.0f / SQRT3);
        d.a = (1 - t4 - t5) / 2.0f;
        d.b = d.a + t4;
        d.c = d.b + t5;
        break;
    }
    case 5:
    {
        float t5 = - v.alpha - (v.beta / SQRT3);;
        float t6 = v.alpha - (v.beta / SQRT3);
        d.b = (1 - t5 - t6) / 2.0f;
        d.a = d.b + t6;
        d.c = d.a + t5;
        break;
    }
    case 6:
    {
        float t6 = - v.beta * (2.0f / SQRT3);
        float t1 = v.alpha + (v.beta / SQRT3);
        d.b = (1 - t6 - t1) / 2.0f;
        d.c = d.b + t6;
        d.a = d.c + t1;
        break;
    }
    default:
        break;
    }

    return d;
}
