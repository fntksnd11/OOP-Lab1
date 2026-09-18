#pragma once
#include "DialogWindow.h"

class ScrollDialog : public DialogWindow {
private:
    int m_selectedValue;

    // Приватна статична процедура обробки (Інкапсуляція Win32 API)
    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

public:
    ScrollDialog(HWND hParent);
    bool Execute() override; // Перевизначення віртуального методу
    int GetValue() const { return m_selectedValue; }
};