#pragma once
#include "Shape.h"

class PointShape : public Shape {
public:
    void Show(HDC hdc) override;
};
