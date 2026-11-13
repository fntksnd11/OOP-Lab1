#include "PointShape.h"

void PointShape::Show(HDC hdc) const {
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(BLACK_BRUSH));

    Ellipse(hdc, xs1 - 2, ys1 - 2, xs1 + 3, ys1 + 3);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
}

void PointShape::Trace(HDC) const {
}
