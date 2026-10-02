#pragma once
#include "DialogWindow.h"

class WizardStep1Dialog : public DialogWindow {
private:
    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

public:
    WizardStep1Dialog(HWND hParent);
    bool Execute() override;
};