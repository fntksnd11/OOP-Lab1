#include "ShapeFile.h"
#include "MyEditor.h"
#include "Shape.h"
#include "ToolRegistry.h"
#include <cstdio>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace {
    const char* const kHeader = "Lab5Shapes 1";
}

bool ShapeFile::Save(const wchar_t* path, const MyEditor& editor) {
    FILE* f = nullptr;
    if (_wfopen_s(&f, path, L"w") != 0 || !f) return false;

    fprintf(f, "%s\n", kHeader);
    for (int i = 0; i < editor.GetCount(); ++i) {
        const Shape* pShape = editor.GetShape(i);
        int kind = pShape->GetKind();
        if (kind < 0 || kind >= ToolRegistry::Count()) continue;

        fprintf(f, "%s %ld %ld %ld %ld\n", ToolRegistry::Get(kind).key,
            pShape->X1(), pShape->Y1(), pShape->X2(), pShape->Y2());
    }
    fclose(f);
    return true;
}

int ShapeFile::Load(const wchar_t* path, MyEditor& editor) {
    FILE* f = nullptr;
    if (_wfopen_s(&f, path, L"r") != 0 || !f) return -1;

    char line[256];
    bool ok = fgets(line, sizeof(line), f) != nullptr
           && std::strncmp(line, kHeader, std::strlen(kHeader)) == 0;

    std::vector<Shape*> loaded;
    while (ok && fgets(line, sizeof(line), f)) {
        std::istringstream iss(line);
        std::string key;
        long x1, y1, x2, y2;
        if (!(iss >> key >> x1 >> y1 >> x2 >> y2)) continue;

        int index = ToolRegistry::IndexByKey(key.c_str());
        if (index < 0) continue;

        Shape* pShape = ToolRegistry::Get(index).create();
        pShape->SetCoord(x1, y1, x2, y2);
        pShape->SetKind(index);
        loaded.push_back(pShape);
    }
    fclose(f);

    if (!ok) return -1;

    editor.Clear();
    int count = 0;
    for (size_t i = 0; i < loaded.size(); ++i) {
        if (editor.AddShape(loaded[i])) ++count;
    }
    return count;
}
