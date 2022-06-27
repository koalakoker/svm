#ifndef SVM_H
#define SVM_H

#include "duty.h"
#include "alphabeta.h"

#define SQRT3 1.73205f

class SVM
{
public:
    SVM();

    static Duty calculate(AlphaBeta v);
    static int sector(AlphaBeta v);
};

#endif // SVM_H
