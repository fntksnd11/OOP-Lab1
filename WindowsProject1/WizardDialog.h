#pragma once
#include "DialogWindow.h"

class WizardDialog : public DialogWindow {
private:
    static INT_PTR CALLBACK Step1Proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
    static INT_PTR CALLBACK Step2Proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

public:
    WizardDialog(HWND hParent) : DialogWindow(hParent) {}
    bool Execute() override;
};
