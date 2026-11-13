#pragma once
#include <windows.h>


class Shape {
protected:
    long xs1, ys1;
    long xs2, ys2;

public:
    Shape();
    virtual ~Shape() = default;

    void SetCoord(long x1, long y1, long x2, long y2);

    virtual void Show(HDC hdc) const = 0;

    virtual void Trace(HDC hdc) const = 0;
};
