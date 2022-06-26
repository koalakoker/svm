#ifndef VLL_H
#define VLL_H

#include "duty.h"

class Vll
{
public:
    Vll();
    Vll(float vdc, Duty d);

    float vab;
    float vbc;
    float vca;
};

#endif // VLL_H
