#pragma once
#include <windows.h>

class DialogWindow {
protected:
    HWND m_hWndParent;

public:
    DialogWindow(HWND hParent) : m_hWndParent(hParent) {}
    virtual ~DialogWindow() = default;

    // „исто в≥ртуальний метод Ч робить клас абстрактним (≤нтерфейсом)
    virtual bool Execute() = 0;
};