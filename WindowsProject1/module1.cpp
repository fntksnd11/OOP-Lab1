#include "module1.h"
#include "resource.h"
#include <commctrl.h>
#include <cwchar>

// Callback-функція ПРИХОВАНА всередині модуля (статична, невидима зовні)
static INT_PTR CALLBACK ScrollDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static int* pResult = nullptr;
    static int currentVal = 50; // значення за замовчуванням

    switch (message) {
    case WM_INITDIALOG: {
        pResult = (int*)lParam;
        HWND hScroll = GetDlgItem(hDlg, IDC_SCROLLBAR);
        SetScrollRange(hScroll, SB_CTL, 1, 100, TRUE);
        SetScrollPos(hScroll, SB_CTL, currentVal, TRUE);

        wchar_t buf[32];
        swprintf_s(buf, L"Значення: %d", currentVal);
        SetDlgItemText(hDlg, IDC_STATIC_VAL, buf);
        return TRUE;
    }
    case WM_HSCROLL: {
        HWND hScroll = (HWND)lParam;
        int pos = GetScrollPos(hScroll, SB_CTL);
        switch (LOWORD(wParam)) {
        case SB_LINELEFT:      pos = max(1, pos - 1); break;
        case SB_LINERIGHT:     pos = min(100, pos + 1); break;
        case SB_PAGELEFT:      pos = max(1, pos - 10); break;
        case SB_PAGERIGHT:     pos = min(100, pos + 10); break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:    pos = HIWORD(wParam); break;
        }
        SetScrollPos(hScroll, SB_CTL, pos, TRUE);
        currentVal = pos;

        wchar_t buf[32];
        swprintf_s(buf, L"Значення: %d", pos);
        SetDlgItemText(hDlg, IDC_STATIC_VAL, buf);
        return TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            if (pResult) *pResult = currentVal;
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

void RunWork1(HWND hWndParent, int& outValue) {
    int resultVal = -1;
    INT_PTR res = DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_SCROLL),
        hWndParent, ScrollDlgProc, (LPARAM)&resultVal);
    if (res == IDOK) {
        outValue = resultVal;
    }
}