#include "ScrollDialog.h"
#include "resource.h"
#include <cwchar>

ScrollDialog::ScrollDialog(HWND hParent)
    : DialogWindow(hParent), m_selectedValue(50) {
}

INT_PTR CALLBACK ScrollDialog::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
   
    ScrollDialog* pThis = reinterpret_cast<ScrollDialog*>(GetWindowLongPtr(hDlg, DWLP_USER));

    switch (message) {
    case WM_INITDIALOG: {
        pThis = reinterpret_cast<ScrollDialog*>(lParam);
        SetWindowLongPtr(hDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pThis));

        HWND hScroll = GetDlgItem(hDlg, IDC_SCROLLBAR);
        SetScrollRange(hScroll, SB_CTL, 1, 100, TRUE);
        SetScrollPos(hScroll, SB_CTL, pThis->m_selectedValue, TRUE);

        wchar_t buf[32];
        swprintf_s(buf, L"Значення: %d", pThis->m_selectedValue);
        SetDlgItemText(hDlg, IDC_STATIC_VAL, buf);
        return TRUE;
    }
    case WM_HSCROLL: {
        if (!pThis) return FALSE;
        HWND hScroll = reinterpret_cast<HWND>(lParam);
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
        pThis->m_selectedValue = pos;

        wchar_t buf[32];
        swprintf_s(buf, L"Значення: %d", pos);
        SetDlgItemText(hDlg, IDC_STATIC_VAL, buf);
        return TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
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

bool ScrollDialog::Execute() {
    INT_PTR result = DialogBoxParam(
        GetModuleHandle(NULL),
        MAKEINTRESOURCE(IDD_DIALOG_SCROLL),
        m_hWndParent,
        ScrollDialog::DlgProc,
        reinterpret_cast<LPARAM>(this) 
    );
    return (result == IDOK);
}   