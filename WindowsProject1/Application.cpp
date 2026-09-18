#include "Application.h"
#include "resource.h"
#include "ScrollDialog.h"
#include "WizardDialog.h"
#include <memory>

Application::Application(HINSTANCE hInstance)
    : m_hInstance(hInstance), m_hWnd(nullptr) {
    wcscpy_s(m_status1, L"Результат Робота1: Не виконано");
    wcscpy_s(m_status2, L"Результат Робота2: Не виконано");
}

bool Application::Initialize() {
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Application::WindowProc;
    wcex.hInstance = m_hInstance;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
    wcex.lpszClassName = L"OOP_WinAppClass";

    if (!RegisterClassEx(&wcex)) return false;

    m_hWnd = CreateWindow(L"OOP_WinAppClass", L"ООП Лабораторна 1", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 400, 300, nullptr, nullptr, m_hInstance, this);

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
    case WM_COMMAND:
        if (pApp) pApp->OnCommand(LOWORD(wp));
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

void Application::OnCommand(WORD id) {
    // Використання поліморфізму через розумні вказівники (std::unique_ptr)
    std::unique_ptr<DialogWindow> dialog;

    if (id == IDM_WORK1) {
        ScrollDialog scrollDlg(m_hWnd);
        if (scrollDlg.Execute()) {
            swprintf_s(m_status1, L"Вибране число (B1): %d", scrollDlg.GetValue());
        }
        InvalidateRect(m_hWnd, NULL, TRUE);
    }
    else if (id == IDM_WORK2) {
        dialog = std::make_unique<WizardDialog>(m_hWnd); // Поліморфний об'єкт
        if (dialog->Execute()) {
            wcscpy_s(m_status2, L"Робота2 успішно завершена");
        }
        else {
            wcscpy_s(m_status2, L"Робота2 скасована");
        }
        InvalidateRect(m_hWnd, NULL, TRUE);
    }
    else if (id == IDM_EXIT) {
        DestroyWindow(m_hWnd);
    }
}

void Application::OnPaint(HDC hdc) {
    TextOut(hdc, 20, 20, m_status1, wcslen(m_status1));
    TextOut(hdc, 20, 50, m_status2, wcslen(m_status2));
}

int Application::Run() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}