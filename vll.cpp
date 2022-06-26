#include "vll.h"

Vll::Vll()
{
    this->vab = 0.0f;
    this->vbc = 0.0f;
    this->vca = 0.0f;
}

Vll::Vll(float vdc, Duty d) {
    this->vab = vdc * (d.a - d.b);
    this->vbc = vdc * (d.b - d.c);
    this->vca = vdc * (d.c - d.a);
}
