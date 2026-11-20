#pragma once

class Shape;

typedef Shape* (*ShapeCreator)();


struct ToolDescriptor {
    int            commandId;   
    int            imageIndex;  
    const wchar_t* name;       
    const wchar_t* tooltip;     
    ShapeCreator   create;    
};


namespace ToolRegistry {
    int  Count();
    const ToolDescriptor& Get(int index);
    int  IndexByCommand(int commandId);  
}
