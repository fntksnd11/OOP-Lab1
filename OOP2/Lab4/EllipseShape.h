#pragma once
#include "Shape.h"

class EllipseShape : public virtual Shape {
public:
    void Show(HDC hdc) override;
    void Trace(HDC hdc) override;
};
