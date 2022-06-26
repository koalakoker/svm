#ifndef SVM_H
#define SVM_H

#include "duty.h"
#include "alphabeta.h"

class SVM
{
public:
    SVM();

    static Duty calculate(AlphaBeta v);
    static int sector(AlphaBeta v);
};

#endif // SVM_H
