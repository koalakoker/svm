#include "duty.h"

Duty::Duty()
{
    this->a = 0.0f;
    this->b = 0.0f;
    this->c = 0.0f;
}

Duty::Duty(float a, float b, float c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

QString Duty::toString() {
    return QString::number(this->a) + " " + QString::number(this->b) + " " + QString::number(this->c);
}
