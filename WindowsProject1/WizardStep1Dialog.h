#pragma once
#include "DialogWindow.h"

class WizardStep1Dialog : public DialogWindow {
private:
    static INT_PTR CALLBACK Step1Proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

public:
    WizardStep1Dialog(HWND hParent) : DialogWindow(hParent) {}
    bool Execute() override;
};