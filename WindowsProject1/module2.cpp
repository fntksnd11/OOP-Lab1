#include "module2.h"
#include "resource.h"

// Попереднє оголошення Callback-функцій (приховані через static)
static INT_PTR CALLBACK Step1DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK Step2DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

static INT_PTR CALLBACK Step1DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_NEXT) {
            EndDialog(hDlg, IDC_BTN_NEXT); // Закриваємо поточне діалогове вікно
            return TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static INT_PTR CALLBACK Step2DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BTN_BACK) {
            EndDialog(hDlg, IDC_BTN_BACK); // Закриваємо вікно з поверненням назад
            return TRUE;
        }
        else if (LOWORD(wParam) == IDOK) {
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void RunWork2(HWND hWndParent, bool& outCompleted) {
    HINSTANCE hInst = GetModuleHandle(NULL);
    outCompleted = false;

    while (true) {
        // Крок 1: Показати перше вікно
        INT_PTR res1 = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_STEP1), hWndParent, Step1DlgProc);
        if (res1 != IDC_BTN_NEXT) {
            break; // Скасовано
        }

        // Крок 2: Автоматично відкрити друге вікно після закриття першого
        INT_PTR res2 = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_STEP2), hWndParent, Step2DlgProc);
        if (res2 == IDC_BTN_BACK) {
            continue; // Повернутися до Кроку 1
        }
        else if (res2 == IDOK) {
            outCompleted = true;
            break;
        }
        else {
            break; // Скасовано у другому вікні
        }
    }
}