#pragma once
#include <windows.h>
#include "Shape.h"
#include "ShapeEditor.h"

const int N = 108;

class Application {
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    Shape* pcshape[N];
    int m_shapeCount;

    ShapeEditor m_editor;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
    void OnCommand(WORD id);
    void OnInitMenuPopup(HMENU hMenu);
    void OnPaint(HDC hdc);

public:
    Application(HINSTANCE hInstance);
    ~Application();

    bool Initialize();
    int Run();

    void AddShape(Shape* pShape);
};