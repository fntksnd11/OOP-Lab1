#include "ToolRegistry.h"
#include "resource.h"
#include "PointShape.h"
#include "LineShape.h"
#include "RectShape.h"
#include "EllipseShape.h"
#include "LineCirclesShape.h"
#include "CubeShape.h"
#include <cstring>

namespace {
    Shape* CreatePoint()       { return new PointShape(); }
    Shape* CreateLine()        { return new LineShape(); }
    Shape* CreateRect()        { return new RectShape(); }
    Shape* CreateEllipse()     { return new EllipseShape(); }
    Shape* CreateLineCircles() { return new LineCirclesShape(); }
    Shape* CreateCube()        { return new CubeShape(); }

    const ToolDescriptor kTools[] = {
        { "Point",       L"Крапка",             L"Крапка",                                  CreatePoint },
        { "Line",        L"Лінія",              L"Лінія",                                   CreateLine },
        { "Rect",        L"Прямокутник",        L"Прямокутник (від центру до кута)",        CreateRect },
        { "Ellipse",     L"Еліпс",              L"Еліпс (по двох протилежних кутах)",       CreateEllipse },
        { "LineCircles", L"Лінія з кружечками", L"Лінія з кружечками",                      CreateLineCircles },
        { "Cube",        L"Каркас куба",        L"Каркас куба (по двох кутах охоплюючого прямокутника)", CreateCube },
    };
    const int kToolCount = sizeof(kTools) / sizeof(kTools[0]);
}

int ToolRegistry::Count() {
    return kToolCount;
}

const ToolDescriptor& ToolRegistry::Get(int index) {
    return kTools[index];
}

int ToolRegistry::CommandId(int index) {
    return IDM_TOOL_FIRST + index;
}

int ToolRegistry::IndexByCommand(int commandId) {
    int index = commandId - IDM_TOOL_FIRST;
    return (index >= 0 && index < kToolCount) ? index : -1;
}

int ToolRegistry::IndexByKey(const char* key) {
    for (int i = 0; i < kToolCount; ++i) {
        if (std::strcmp(kTools[i].key, key) == 0) return i;
    }
    return -1;
}
