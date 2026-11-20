#pragma once
#include "Shape.h"

class LineShape : public virtual Shape {
public:
    void Show(HDC hdc) override;
    void Trace(HDC hdc) override;
};
