#pragma once
#include <windows.h>
#include <cwchar>

class Application {
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    wchar_t m_status1[128];
    wchar_t m_status2[128];

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
    void OnCommand(WORD id);
    void OnPaint(HDC hdc);

public:
    Application(HINSTANCE hInstance);
    bool Initialize();
    int Run();
};