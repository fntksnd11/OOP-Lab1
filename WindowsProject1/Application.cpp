#include "Application.h"
#include "resource.h"

Application::Application(HINSTANCE hInstance)
    : m_hInstance(hInstance), m_hWnd(nullptr), m_shapeCount(0) {
    for (int i = 0; i < N; ++i) {
        pcshape[i] = nullptr;
    }
}

Application::~Application() {
    for (int i = 0; i < m_shapeCount; ++i) {
        delete pcshape[i];
        pcshape[i] = nullptr;
    }
}

void Application::AddShape(Shape* pShape) {
    if (m_shapeCount < N) {
        pcshape[m_shapeCount++] = pShape;
    }
    else {
        delete pShape;
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
    wcex.lpszClassName = L"Lab2_EditorClass";

    if (!RegisterClassEx(&wcex)) return false;

    m_hWnd = CreateWindow(
        L"Lab2_EditorClass",
        L"Лабораторна робота №2 - Графічний редактор (Варіант 8)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 800, 600,
        nullptr, nullptr, m_hInstance, this
    );

    if (!m_hWnd) return false;

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    return true;
}

LRESULT CALLBACK Application::WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    Application* pApp = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (msg) {
    case WM_CREATE: {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lp);
        pApp = reinterpret_cast<Application*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));
        return 0;
    }
    case WM_INITMENUPOPUP:
        if (pApp) pApp->OnInitMenuPopup((HMENU)wp);
        return 0;

    case WM_COMMAND:
        if (pApp) pApp->OnCommand(LOWORD(wp));
        return 0;

    case WM_LBUTTONDOWN:
        if (pApp) pApp->m_editor.OnLButtonDown(hWnd, lp);
        return 0;

    case WM_MOUSEMOVE:
        if (pApp) pApp->m_editor.OnMouseMove(hWnd, lp);
        return 0;

    case WM_LBUTTONUP:
        if (pApp) pApp->m_editor.OnLButtonUp(hWnd, lp, pApp);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (pApp) pApp->OnPaint(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wp, lp);
}

void Application::OnInitMenuPopup(HMENU hMenu) {
    CheckMenuItem(hMenu, IDM_OBJECT_POINT, MF_BYCOMMAND | (m_editor.GetTool() == TOOL_POINT ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_OBJECT_LINE, MF_BYCOMMAND | (m_editor.GetTool() == TOOL_LINE ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_OBJECT_RECT, MF_BYCOMMAND | (m_editor.GetTool() == TOOL_RECT ? MF_CHECKED : MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_OBJECT_ELLIPSE, MF_BYCOMMAND | (m_editor.GetTool() == TOOL_ELLIPSE ? MF_CHECKED : MF_UNCHECKED));
}

void Application::OnCommand(WORD id) {
    switch (id) {
    case IDM_OBJECT_POINT:   m_editor.SetTool(TOOL_POINT); break;
    case IDM_OBJECT_LINE:    m_editor.SetTool(TOOL_LINE); break;
    case IDM_OBJECT_RECT:    m_editor.SetTool(TOOL_RECT); break;
    case IDM_OBJECT_ELLIPSE: m_editor.SetTool(TOOL_ELLIPSE); break;
    case IDM_FILE_EXIT:      DestroyWindow(m_hWnd); break;
    }
}

void Application::OnPaint(HDC hdc) {
    for (int i = 0; i < m_shapeCount; ++i) {
        if (pcshape[i]) {
            pcshape[i]->Show(hdc);
        }
    }
}

int Application::Run() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}