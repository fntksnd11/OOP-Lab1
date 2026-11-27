#pragma once
#include "Shape.h"

class RectShape : public virtual Shape {
public:
    void Show(HDC hdc) override;
    void Trace(HDC hdc) override;
};
