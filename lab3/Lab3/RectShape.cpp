#include "RectShape.h"
#include <cstdlib>

void RectShape::Show(HDC hdc) const {
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Trace(hdc);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
}

void RectShape::Trace(HDC hdc) const {
    long dx = std::labs(xs2 - xs1);
    long dy = std::labs(ys2 - ys1);
    Rectangle(hdc, xs1 - dx, ys1 - dy, xs1 + dx, ys1 + dy);
}
