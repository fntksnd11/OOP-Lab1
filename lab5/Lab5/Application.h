#pragma once
#include <windows.h>
#include "MyEditor.h"
#include "Toolbar.h"
#include "my_table.h"

class Application : public ITableListener {
private:
    HINSTANCE m_hInstance;
    HWND      m_hWnd;

    MyEditor* m_pEditor;
    Toolbar   m_toolbar;
    MyTable   m_table;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

    bool    OnCreate();
    void    OnDestroy();
    void    OnSize();
    void    OnCommand(WORD id);
    LRESULT OnNotify(LPARAM lp);
    void    OnInitMenuPopup(HMENU hMenu);
    void    OnPaint();
    void    OnLButtonDown(LPARAM lp);
    void    OnMouseMove(LPARAM lp);
    void    OnLButtonUp(LPARAM lp);
    void    OnCaptureChanged();

    void OnFileOpen();
    void OnFileSave();
    void ToggleTable();

    void OnRowSelected(int rowIndex) override;
    void OnRowDeleteRequested(int rowIndex) override;
    void OnTableClosed() override;

    void BuildObjectsMenu();
    void SetTool(int toolIndex);
    void UpdateTitle();
    void FillTable();
    void AddTableRow(const Shape* pShape);

public:
    explicit Application(HINSTANCE hInstance);
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    bool Initialize();
    int  Run();
};
