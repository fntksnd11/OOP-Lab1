#pragma once
#include "LineShape.h"
#include "EllipseShape.h"

class LineCirclesShape : public LineShape, public EllipseShape {
private:
    void Render(HDC hdc, bool trace);

public:
    void Show(HDC hdc) override;
    void Trace(HDC hdc) override;
};
