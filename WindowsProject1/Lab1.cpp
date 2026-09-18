#include <windows.h>
#include <cwchar>
#include "resource.h"
#include "module1.h"
#include "module2.h"

static wchar_t g_Work1Text[128] = L"Результат Робота1: Не виконано";
static wchar_t g_Work2Text[128] = L"Результат Робота2: Не виконано";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_WORK1: {
            int val = -1;
            RunWork1(hWnd, val);
            if (val != -1) {
                swprintf_s(g_Work1Text, L"Вибране число (B1): %d", val);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        }
        case IDM_WORK2: {
            bool ok = false;
            RunWork2(hWnd, ok);
            if (ok) {
                swprintf_s(g_Work2Text, L"Робота2 успішно завершена (B2=2)");
            }
            else {
                swprintf_s(g_Work2Text, L"Робота2 скасована");
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 20, 20, g_Work1Text, wcslen(g_Work1Text));
        TextOut(hdc, 20, 50, g_Work2Text, wcslen(g_Work2Text));
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
    wcex.lpszClassName = L"Lab1WindowClass";

    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(L"Lab1WindowClass", L"Лабораторна 1 - Варіант 1",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}