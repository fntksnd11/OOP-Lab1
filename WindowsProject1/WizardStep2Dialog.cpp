#include "WizardStep2Dialog.h"
#include "resource.h"

WizardStep2Dialog::WizardStep2Dialog(HWND hParent)
    : DialogWindow(hParent) {
}

INT_PTR CALLBACK WizardStep2Dialog::DlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_COMMAND) {
        if (LOWORD(wp) == IDC_BTN_BACK) {
            EndDialog(hDlg, IDC_BTN_BACK);
            return TRUE;
        }
        if (LOWORD(wp) == IDOK) {
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        if (LOWORD(wp) == IDCANCEL) {
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
    }
    return FALSE;
}

Step2Result WizardStep2Dialog::ExecuteStep() {
    INT_PTR res = DialogBox(
        GetModuleHandle(NULL),
        MAKEINTRESOURCE(IDD_DIALOG_STEP2),
        m_hWndParent,
        WizardStep2Dialog::DlgProc
    );

    if (res == IDC_BTN_BACK) return Step2Result::Back;
    if (res == IDOK) return Step2Result::Success;
    return Step2Result::Cancel;
}

bool WizardStep2Dialog::Execute() {
    return (ExecuteStep() == Step2Result::Success);
}