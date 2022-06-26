#ifndef DUTY_H
#define DUTY_H

#include <QString>

class Duty
{
public:
    Duty();
    Duty(float a, float b, float c);
    QString toString();

    float a;
    float b;
    float c;
};

#endif // DUTY_H
