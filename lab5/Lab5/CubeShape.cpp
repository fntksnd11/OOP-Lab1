#include "CubeShape.h"

void CubeShape::Show(HDC hdc) {
    Render(hdc, false);
}

void CubeShape::Trace(HDC hdc) {
    Render(hdc, true);
}

void CubeShape::DrawRect(HDC hdc, bool trace, long x, long y, long halfW, long halfH) {
    SetCoord(x + halfW, y + halfH, x + 2 * halfW, y + 2 * halfH);
    if (trace) RectShape::Trace(hdc); else RectShape::Show(hdc);
}

void CubeShape::DrawEdge(HDC hdc, bool trace, long ax, long ay, long bx, long by) {
    SetCoord(ax, ay, bx, by);
    if (trace) LineShape::Trace(hdc); else LineShape::Show(hdc);
}

void CubeShape::Render(HDC hdc, bool trace) {
    const long x1 = xs1, y1 = ys1, x2 = xs2, y2 = ys2;

    long left   = (x1 < x2) ? x1 : x2;
    long right  = (x1 < x2) ? x2 : x1;
    long top    = (y1 < y2) ? y1 : y2;
    long bottom = (y1 < y2) ? y2 : y1;

    long w = right - left;
    long h = bottom - top;
    long side = (w < h) ? w : h;
    long d = side / 4;
    long halfW = (w - d) / 2;
    long halfH = (h - d) / 2;
    long fw = 2 * halfW;
    long fh = 2 * halfH;

    DrawRect(hdc, trace, left, top + d, halfW, halfH);
    DrawRect(hdc, trace, left + d, top, halfW, halfH);

    DrawEdge(hdc, trace, left,      top + d,      left + d,      top);
    DrawEdge(hdc, trace, left + fw, top + d,      left + d + fw, top);
    DrawEdge(hdc, trace, left,      top + d + fh, left + d,      top + fh);
    DrawEdge(hdc, trace, left + fw, top + d + fh, left + d + fw, top + fh);

    SetCoord(x1, y1, x2, y2);
}
