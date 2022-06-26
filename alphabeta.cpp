#include "alphabeta.h"

AlphaBeta::AlphaBeta()
{
    this->alpha = 0;
    this->beta = 0;
}

AlphaBeta::AlphaBeta(float alpha, float beta)
{
    this->alpha = alpha;
    this->beta = beta;
}

QString AlphaBeta::toString() {
    return QString::number(this->alpha) + " " + QString::number(this->beta);
}
