#include "Application.h"
#include "resource.h"
#include <windowsx.h>
#include <commctrl.h>
#include <string>

#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace {
    const wchar_t* const kClassName = L"Lab3_EditorClass";
    const wchar_t* const kTitleBase = L"Лабораторна робота №3 - Графічний редактор (Варіант 9)";

    int ToolToCommand(CurrentTool tool) {
        switch (tool) {
        case TOOL_POINT:   return IDM_OBJECT_POINT;
        case TOOL_LINE:    return IDM_OBJECT_LINE;
        case TOOL_RECT:    return IDM_OBJECT_RECT;
        case TOOL_ELLIPSE: return IDM_OBJECT_ELLIPSE;
        }
        return 0;
    }
}

Application::Application(HINSTANCE hInstance)
    : m_hInstance(hInstance), m_hWnd(nullptr), pcshape(nullptr), m_shapeCount(0) {
    pcshape = new Shape*[N];
    for (int i = 0; i < N; ++i) {
        pcshape[i] = nullptr;
    }
}

Application::~Application() {
    for (int i = 0; i < m_shapeCount; ++i) {
        delete pcshape[i];
    }
    delete[] pcshape;
}

void Application::AddShape(Shape* pShape) {
    if (m_shapeCount < N) {
        pcshape[m_shapeCount++] = pShape;
    }
    else {
        delete pShape;
        MessageBox(m_hWnd, L"Масив заповнено: досягнуто максимальної кількості об'єктів (109).",
            L"Графічний редактор", MB_OK | MB_ICONINFORMATION);
    }
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
        CW_USEDEFAULT, 0, 800, 600,
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
    SetTool(TOOL_POINT);  
    return true;
}

void Application::OnSize() {
    m_toolbar.AutoSize();
}

void Application::OnCommand(WORD id) {
    switch (id) {
    case IDM_OBJECT_POINT:   SetTool(TOOL_POINT);   break;
    case IDM_OBJECT_LINE:    SetTool(TOOL_LINE);    break;
    case IDM_OBJECT_RECT:    SetTool(TOOL_RECT);    break;
    case IDM_OBJECT_ELLIPSE: SetTool(TOOL_ELLIPSE); break;
    case IDM_FILE_EXIT:      DestroyWindow(m_hWnd); break;
    case IDM_HELP_ABOUT:
        MessageBox(m_hWnd,
            L"Лабораторна робота №3\nГрафічний редактор об'єктів з панеллю інструментів\nВаріант 9",
            L"Про програму", MB_OK | MB_ICONINFORMATION);
        break;
    }
}

LRESULT Application::OnNotify(LPARAM lp) {
    m_toolbar.OnNotify(reinterpret_cast<LPNMHDR>(lp));
    return 0;
}

void Application::OnInitMenuPopup(HMENU hMenu) {
    CurrentTool tool = m_editor.GetTool();
    CheckMenuItem(hMenu, IDM_OBJECT_POINT,   MF_BYCOMMAND | (tool == TOOL_POINT   ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_OBJECT_LINE,    MF_BYCOMMAND | (tool == TOOL_LINE    ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_OBJECT_RECT,    MF_BYCOMMAND | (tool == TOOL_RECT    ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_OBJECT_ELLIPSE, MF_BYCOMMAND | (tool == TOOL_ELLIPSE ? MF_CHECKED : MF_UNCHECKED));
}

void Application::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);
    for (int i = 0; i < m_shapeCount; ++i) {
        pcshape[i]->Show(hdc);     
    }
    EndPaint(m_hWnd, &ps);
}

void Application::OnLButtonDown(LPARAM lp) {
    m_editor.OnLButtonDown(m_hWnd, GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
}

void Application::OnMouseMove(LPARAM lp) {
    m_editor.OnMouseMove(m_hWnd, GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
}

void Application::OnLButtonUp(LPARAM lp) {
    Shape* pShape = m_editor.OnLButtonUp(m_hWnd, GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
    if (pShape) {
        AddShape(pShape);
        InvalidateRect(m_hWnd, NULL, TRUE);
    }
}

void Application::OnCaptureChanged() {
    m_editor.OnCaptureLost(m_hWnd);
}

void Application::SetTool(CurrentTool tool) {
    m_editor.SetTool(tool);
    m_toolbar.CheckTool(ToolToCommand(tool));
    UpdateTitle();
}

void Application::UpdateTitle() {
    std::wstring title = kTitleBase;
    title += L" - Об'єкт: ";
    title += ShapeEditor::ToolName(m_editor.GetTool());
    SetWindowText(m_hWnd, title.c_str());
}
