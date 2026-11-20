#include "Application.h"
#include "ToolRegistry.h"
#include "resource.h"
#include <windowsx.h>
#include <commctrl.h>
#include <string>

#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace {
    const wchar_t* const kClassName = L"Lab4_EditorClass";
    const wchar_t* const kTitleBase = L"Лабораторна робота №4 - Графічний редактор";
}

Application::Application(HINSTANCE hInstance)
    : m_hInstance(hInstance), m_hWnd(nullptr), m_pEditor(nullptr) {
    m_pEditor = new MyEditor();    
}

Application::~Application() {
    delete m_pEditor;              
    m_pEditor = nullptr;
}

bool Application::Initialize() {
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Application::WindowProc;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
    wcex.lpszClassName = kClassName;

    if (!RegisterClassEx(&wcex)) return false;

    
    m_hWnd = CreateWindow(
        kClassName, kTitleBase,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, 0, 900, 600,
        nullptr, nullptr, m_hInstance, this);

    if (!m_hWnd) return false;

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
    return true;
}

int Application::Run() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK Application::WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    Application* pApp = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (msg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lp);
        pApp = reinterpret_cast<Application*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));
        pApp->m_hWnd = hWnd;
    }

    if (!pApp) return DefWindowProc(hWnd, msg, wp, lp);
    return pApp->HandleMessage(hWnd, msg, wp, lp);
}

LRESULT Application::HandleMessage(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        return OnCreate() ? 0 : -1;

    case WM_SIZE:
        OnSize();
        return 0;

    case WM_COMMAND:
        OnCommand(LOWORD(wp));
        return 0;

    case WM_NOTIFY:
        return OnNotify(lp);

    case WM_INITMENUPOPUP:
        OnInitMenuPopup(reinterpret_cast<HMENU>(wp));
        return 0;

    case WM_PAINT:
        OnPaint();
        return 0;

    case WM_LBUTTONDOWN:
        OnLButtonDown(lp);
        return 0;

    case WM_MOUSEMOVE:
        OnMouseMove(lp);
        return 0;

    case WM_LBUTTONUP:
        OnLButtonUp(lp);
        return 0;

    case WM_CAPTURECHANGED:
        OnCaptureChanged();
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wp, lp);
}

bool Application::OnCreate() {
    if (!m_toolbar.Create(m_hWnd, m_hInstance)) return false;
    SetTool(0); 
    return true;
}

void Application::OnSize() {
    m_toolbar.AutoSize();
}

void Application::OnCommand(WORD id) {
    switch (id) {
    case IDM_FILE_EXIT:
        DestroyWindow(m_hWnd);
        return;
    case IDM_HELP_ABOUT:
        MessageBox(m_hWnd,
            L"Лабораторна робота №4\nГрафічний редактор: 6 типів об'єктів,\nмножинне успадкування, реєстр інструментів",
            L"Про програму", MB_OK | MB_ICONINFORMATION);
        return;
    }

   
    int index = ToolRegistry::IndexByCommand(id);
    if (index >= 0) SetTool(index);
}


LRESULT Application::OnNotify(LPARAM lp) {
    m_toolbar.OnNotify(reinterpret_cast<LPNMHDR>(lp));
    return 0;
}

void Application::OnInitMenuPopup(HMENU hMenu) {
    int current = m_pEditor->GetTool();
    for (int i = 0; i < ToolRegistry::Count(); ++i) {
        CheckMenuItem(hMenu, ToolRegistry::Get(i).commandId,
            MF_BYCOMMAND | (i == current ? MF_CHECKED : MF_UNCHECKED));
    }
}

void Application::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);
    m_pEditor->OnPaint(hdc);
    EndPaint(m_hWnd, &ps);
}

void Application::OnLButtonDown(LPARAM lp) {
    m_pEditor->OnLButtonDown(m_hWnd, GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
}

void Application::OnMouseMove(LPARAM lp) {
    m_pEditor->OnMouseMove(m_hWnd, GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
}

void Application::OnLButtonUp(LPARAM lp) {
    EditResult result = m_pEditor->OnLButtonUp(m_hWnd, GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
    if (result == EDIT_ADDED) {
        InvalidateRect(m_hWnd, NULL, TRUE);
    }
    else if (result == EDIT_FULL) {
        MessageBox(m_hWnd, L"Масив заповнено: досягнуто максимальної кількості об'єктів (109).",
            L"Графічний редактор", MB_OK | MB_ICONINFORMATION);
    }
}

void Application::OnCaptureChanged() {
    m_pEditor->OnCaptureLost(m_hWnd);
}

void Application::SetTool(int toolIndex) {
    m_pEditor->SetTool(toolIndex);
    m_toolbar.CheckTool(ToolRegistry::Get(toolIndex).commandId);
    UpdateTitle();
}

void Application::UpdateTitle() {
    std::wstring title = kTitleBase;
    title += L" - Об'єкт: ";
    title += ToolRegistry::Get(m_pEditor->GetTool()).name;
    SetWindowText(m_hWnd, title.c_str());
}
