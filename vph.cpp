#include "vph.h"

Vph::Vph()
{
    this->va = 0.0f;
    this->vb = 0.0f;
    this->vc = 0.0f;
}

Vph::Vph(Vll vll) {
    this->va = ((2.0f * vll.vab) + vll.vbc) / 3.0f;
    this->vb = (-vll.vab + vll.vbc) / 3.0f;
    this->vc = (-vll.vab - (2.0f * vll.vbc)) / 3.0f;
}
