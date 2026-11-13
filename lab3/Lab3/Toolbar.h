#pragma once
#include <windows.h>
#include <commctrl.h>

class Toolbar {
private:
    HWND   m_hWnd;
    HIMAGELIST m_hImageList;

    static const wchar_t* TooltipText(int commandId);

public:
    Toolbar();
    ~Toolbar();
    Toolbar(const Toolbar&) = delete;
    Toolbar& operator=(const Toolbar&) = delete;

    bool Create(HWND hParent, HINSTANCE hInstance);
    void AutoSize();
    void CheckTool(int commandId); 

  
    bool OnNotify(LPNMHDR pnmh);
};
