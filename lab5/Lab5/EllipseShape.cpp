#include "EllipseShape.h"

void EllipseShape::Show(HDC hdc) {
    HBRUSH hBrush = CreateSolidBrush(RGB(135, 206, 235));
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    EllipseShape::Trace(hdc);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
}

void EllipseShape::Trace(HDC hdc) {
    long left   = (xs1 < xs2) ? xs1 : xs2;
    long right  = (xs1 < xs2) ? xs2 : xs1;
    long top    = (ys1 < ys2) ? ys1 : ys2;
    long bottom = (ys1 < ys2) ? ys2 : ys1;
    Ellipse(hdc, left, top, right, bottom);
}
