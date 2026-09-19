#include "WizardDialog.h"
#include "resource.h"

INT_PTR CALLBACK WizardDialog::Step1Proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_COMMAND) {
        if (LOWORD(wp) == IDC_BTN_NEXT) { EndDialog(hDlg, IDC_BTN_NEXT); return TRUE; }
        if (LOWORD(wp) == IDCANCEL) { EndDialog(hDlg, IDCANCEL); return TRUE; }
    }
    return FALSE;
}

INT_PTR CALLBACK WizardDialog::Step2Proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_COMMAND) {
        if (LOWORD(wp) == IDC_BTN_BACK) { EndDialog(hDlg, IDC_BTN_BACK); return TRUE; }
        if (LOWORD(wp) == IDOK) { EndDialog(hDlg, IDOK); return TRUE; }
        if (LOWORD(wp) == IDCANCEL) { EndDialog(hDlg, IDCANCEL); return TRUE; }
    }
    return FALSE;
}

bool WizardDialog::Execute() {
    HINSTANCE hInst = GetModuleHandle(NULL);
    while (true) {
        INT_PTR res1 = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_STEP1), m_hWndParent, Step1Proc);
        if (res1 != IDC_BTN_NEXT) return false;

        INT_PTR res2 = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_STEP2), m_hWndParent, Step2Proc);
        if (res2 == IDC_BTN_BACK) continue; 
        return (res2 == IDOK);
    }
}