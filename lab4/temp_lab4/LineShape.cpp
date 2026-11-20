#include "LineShape.h"

void LineShape::Show(HDC hdc) {
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));

    LineShape::Trace(hdc);

    SelectObject(hdc, hOldPen);
}

void LineShape::Trace(HDC hdc) {
    MoveToEx(hdc, xs1, ys1, NULL);
    LineTo(hdc, xs2, ys2);
}
