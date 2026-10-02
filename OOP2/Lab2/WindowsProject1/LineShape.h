#pragma once
#include "Shape.h"

class LineShape : public Shape {
public:
    void Show(HDC hdc) override;
};
