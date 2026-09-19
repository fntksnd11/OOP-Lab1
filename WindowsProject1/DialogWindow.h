#pragma once
#include <windows.h>

class DialogWindow {
protected:
    HWND m_hWndParent;

public:
    DialogWindow(HWND hParent) : m_hWndParent(hParent) {}
    virtual ~DialogWindow() = default;

 
    virtual bool Execute() = 0;
};