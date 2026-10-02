#pragma once
#include "Shape.h"

class EllipseShape : public Shape {
public:
    void Show(HDC hdc) override;
};
