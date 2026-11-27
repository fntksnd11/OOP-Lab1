#pragma once
#include "Shape.h"

class PointShape : public virtual Shape {
public:
    void Show(HDC hdc) override;
    void Trace(HDC hdc) override;
    void Highlight(HDC hdc) override;
};
