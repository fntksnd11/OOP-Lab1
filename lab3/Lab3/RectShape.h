#pragma once
#include "Shape.h"

class RectShape : public Shape {
public:
    void Show(HDC hdc) const override;
    void Trace(HDC hdc) const override;
};
