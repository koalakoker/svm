#ifndef ALPHABETA_H
#define ALPHABETA_H

#include <QString>

class AlphaBeta
{
public:
    AlphaBeta();
    AlphaBeta(float alpha, float beta);
    QString toString();

    float alpha;
    float beta;
};

#endif // ALPHABETA_H
