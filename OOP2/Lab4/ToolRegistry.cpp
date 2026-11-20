#include "ToolRegistry.h"
#include "resource.h"
#include "PointShape.h"
#include "LineShape.h"
#include "RectShape.h"
#include "EllipseShape.h"
#include "LineCirclesShape.h"
#include "CubeShape.h"

namespace {
    Shape* CreatePoint()       { return new PointShape(); }
    Shape* CreateLine()        { return new LineShape(); }
    Shape* CreateRect()        { return new RectShape(); }
    Shape* CreateEllipse()     { return new EllipseShape(); }
    Shape* CreateLineCircles() { return new LineCirclesShape(); }
    Shape* CreateCube()        { return new CubeShape(); }


    const ToolDescriptor kTools[] = {
        { IDM_OBJECT_POINT,        0, L"Крапка",             L"Крапка",                                  CreatePoint },
        { IDM_OBJECT_LINE,         1, L"Лінія",              L"Лінія",                                   CreateLine },
        { IDM_OBJECT_RECT,         2, L"Прямокутник",        L"Прямокутник (від центру до кута)",        CreateRect },
        { IDM_OBJECT_ELLIPSE,      3, L"Еліпс",              L"Еліпс (по двох протилежних кутах)",       CreateEllipse },
        { IDM_OBJECT_LINE_CIRCLES, 4, L"Лінія з кружечками", L"Лінія з кружечками",                      CreateLineCircles },
        { IDM_OBJECT_CUBE,         5, L"Каркас куба",        L"Каркас куба (по двох кутах охоплюючого прямокутника)", CreateCube },
    };
    const int kToolCount = sizeof(kTools) / sizeof(kTools[0]);
}

int ToolRegistry::Count() {
    return kToolCount;
}

const ToolDescriptor& ToolRegistry::Get(int index) {
    return kTools[index];
}

int ToolRegistry::IndexByCommand(int commandId) {
    for (int i = 0; i < kToolCount; ++i) {
        if (kTools[i].commandId == commandId) return i;
    }
    return -1;
}
