#include "LineCirclesShape.h"

namespace {
    const long kRadius = 6;  
}


void LineCirclesShape::Render(HDC hdc, bool trace) {
   
    const long x1 = xs1, y1 = ys1, x2 = xs2, y2 = ys2;

   
    if (trace) LineShape::Trace(hdc); else LineShape::Show(hdc);

   
    SetCoord(x1 - kRadius, y1 - kRadius, x1 + kRadius, y1 + kRadius);
    if (trace) EllipseShape::Trace(hdc); else EllipseShape::Show(hdc);

    
    SetCoord(x2 - kRadius, y2 - kRadius, x2 + kRadius, y2 + kRadius);
    if (trace) EllipseShape::Trace(hdc); else EllipseShape::Show(hdc);


    SetCoord(x1, y1, x2, y2);
}

void LineCirclesShape::Show(HDC hdc) {
    Render(hdc, false);
}

void LineCirclesShape::Trace(HDC hdc) {
    Render(hdc, true);
}
