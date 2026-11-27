#pragma once
#include <windows.h>

class Shape {
protected:
    long xs1, ys1;
    long xs2, ys2;
    int  m_kind;

public:
    Shape();
    virtual ~Shape() = default;

    void SetCoord(long x1, long y1, long x2, long y2);

    long X1() const { return xs1; }
    long Y1() const { return ys1; }
    long X2() const { return xs2; }
    long Y2() const { return ys2; }

    void SetKind(int kind) { m_kind = kind; }
    int  GetKind() const { return m_kind; }

    virtual void Show(HDC hdc) = 0;

    virtual void Trace(HDC hdc) = 0;

    virtual void Highlight(HDC hdc);
};
