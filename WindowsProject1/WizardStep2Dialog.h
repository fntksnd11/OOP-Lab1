#pragma once
#include "DialogWindow.h"

enum class Step2Result {
    Back,
    Success,
    Cancel
};

class WizardStep2Dialog : public DialogWindow {
private:
    static INT_PTR CALLBACK Step2Proc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

public:
    WizardStep2Dialog(HWND hParent) : DialogWindow(hParent) {}
    bool Execute() override;        
    Step2Result ExecuteStep();    
};