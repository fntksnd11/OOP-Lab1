#include "WizardStep2Dialog.h"
#include "resource.h"

INT_PTR CALLBACK WizardStep2Dialog::Step2Proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_COMMAND) {
        if (LOWORD(wp) == IDC_BTN_BACK) { EndDialog(hDlg, IDC_BTN_BACK); return TRUE; }
        if (LOWORD(wp) == IDOK) { EndDialog(hDlg, IDOK); return TRUE; }
        if (LOWORD(wp) == IDCANCEL) { EndDialog(hDlg, IDCANCEL); return TRUE; }
    }
    return FALSE;
}

bool WizardStep2Dialog::Execute() {
    return (ExecuteStep() == Step2Result::Success);
}


Step2Result WizardStep2Dialog::ExecuteStep() {
    INT_PTR result = DialogBox(
        GetModuleHandle(NULL),
        MAKEINTRESOURCE(IDD_DIALOG_STEP2),
        m_hWndParent,
        Step2Proc
    );

    if (result == IDC_BTN_BACK) return Step2Result::Back;
    if (result == IDOK) return Step2Result::Success;
    return Step2Result::Cancel;
}