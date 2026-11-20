#pragma once
#include "LineShape.h"
#include "RectShape.h"

class CubeShape : public LineShape, public RectShape {
private:
    void Render(HDC hdc, bool trace);
    void DrawRect(HDC hdc, bool trace, long x, long y, long halfW, long halfH);
    void DrawEdge(HDC hdc, bool trace, long ax, long ay, long bx, long by);

public:
    void Show(HDC hdc) override;
    void Trace(HDC hdc) override;
};
