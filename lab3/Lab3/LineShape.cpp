#include "LineShape.h"

void LineShape::Show(HDC hdc) const {
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));

    Trace(hdc);

    SelectObject(hdc, hOldPen);
}

void LineShape::Trace(HDC hdc) const {
    MoveToEx(hdc, xs1, ys1, NULL);
    LineTo(hdc, xs2, ys2);
}
