#pragma once
#include <windows.h>
#include "Shape.h"
#include "ShapeEditor.h"
#include "Toolbar.h"

const int N = 109;

class Application {
private:
    HINSTANCE m_hInstance;
    HWND      m_hWnd;

    Shape**   pcshape;        
    int       m_shapeCount;

    ShapeEditor m_editor;
    Toolbar     m_toolbar;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

    bool    OnCreate();
    void    OnSize();
    void    OnCommand(WORD id);
    LRESULT OnNotify(LPARAM lp);
    void    OnInitMenuPopup(HMENU hMenu);
    void    OnPaint();
    void    OnLButtonDown(LPARAM lp);
    void    OnMouseMove(LPARAM lp);
    void    OnLButtonUp(LPARAM lp);
    void    OnCaptureChanged();

    void SetTool(CurrentTool tool);
    void UpdateTitle();
    void AddShape(Shape* pShape);

public:
    explicit Application(HINSTANCE hInstance);
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool Initialize();
    int  Run();
};
