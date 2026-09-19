#include "WizardStep1Dialog.h"
#include "resource.h"

INT_PTR CALLBACK WizardStep1Dialog::Step1Proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_COMMAND) {
        if (LOWORD(wp) == IDC_BTN_NEXT) { EndDialog(hDlg, IDC_BTN_NEXT); return TRUE; }
        if (LOWORD(wp) == IDCANCEL) { EndDialog(hDlg, IDCANCEL); return TRUE; }
    }
    return FALSE;
}

bool WizardStep1Dialog::Execute() {
    INT_PTR result = DialogBox(
        GetModuleHandle(NULL),
        MAKEINTRESOURCE(IDD_DIALOG_STEP1),
        m_hWndParent,
        Step1Proc
    );
    return (result == IDC_BTN_NEXT);
}