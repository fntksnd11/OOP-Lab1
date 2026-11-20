#pragma once
#include <windows.h>
#include "MyEditor.h"
#include "Toolbar.h"

class Application {
private:
    HINSTANCE m_hInstance;
    HWND      m_hWnd;

    MyEditor* m_pEditor;     
    Toolbar   m_toolbar;

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

    void SetTool(int toolIndex);
    void UpdateTitle();

public:
    explicit Application(HINSTANCE hInstance);
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool Initialize();
    int  Run();
};
