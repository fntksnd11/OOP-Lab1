#pragma once

class Shape;

typedef Shape* (*ShapeCreator)();

struct ToolDescriptor {
    const char*    key;
    const wchar_t* name;
    const wchar_t* tooltip;
    ShapeCreator   create;
};

namespace ToolRegistry {
    int  Count();
    const ToolDescriptor& Get(int index);
    int  CommandId(int index);
    int  IndexByCommand(int commandId);
    int  IndexByKey(const char* key);
}
