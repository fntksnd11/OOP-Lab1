#pragma once
#include <windows.h>

class ITableListener {
public:
    virtual ~ITableListener() = default;
    virtual void OnRowSelected(int rowIndex) = 0;
    virtual void OnRowDeleteRequested(int rowIndex) = 0;
    virtual void OnTableClosed() = 0;
};

class MyTable {
private:
    HWND            m_hDlg;
    HWND            m_hList;
    HWND            m_hDelete;
    ITableListener* m_pListener;
    bool            m_updating;

    static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

    bool    OnInitDialog(HWND hDlg);
    void    OnSize();
    void    OnCommand(WORD id, WORD code);
    void    OnNotify(LPARAM lp);
    void    OnClose();
    void    OnDestroy();

    int  SelectedRow() const;
    void RequestDelete();
    void Renumber(int fromRow);

public:
    MyTable();
    ~MyTable();
    MyTable(const MyTable&) = delete;
    MyTable& operator=(const MyTable&) = delete;

    void SetListener(ITableListener* pListener) { m_pListener = pListener; }

    bool Open(HWND hParent, HINSTANCE hInstance);
    void Close();
    bool IsOpen() const { return m_hDlg != nullptr; }

    bool ProcessMessage(MSG* pMsg);

    void Clear();
    void AddRow(const wchar_t* typeName, long x1, long y1, long x2, long y2);
    void RemoveRow(int rowIndex);
    void SelectRow(int rowIndex);
};
