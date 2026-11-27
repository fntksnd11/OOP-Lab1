#pragma once

class MyEditor;

namespace ShapeFile {
    bool Save(const wchar_t* path, const MyEditor& editor);
    int  Load(const wchar_t* path, MyEditor& editor);
}
