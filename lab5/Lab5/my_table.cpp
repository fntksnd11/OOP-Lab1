#include "my_table.h"
#include <commctrl.h>
#include <cstring>
#include <cwchar>

#pragma comment(lib, "comctl32.lib")

namespace {
    const int IDC_TABLE_LIST   = 101;
    const int IDC_TABLE_DELETE = 102;

    void AddColumn(HWND hList, int index, const wchar_t* title, int width) {
        LVCOLUMN col = {};
        col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        col.pszText = const_cast<LPWSTR>(title);
        col.cx = width;
        col.iSubItem = index;
        ListView_InsertColumn(hList, index, &col);
    }

    void SetCellNumber(HWND hList, int row, int column, long value) {
        wchar_t buf[32];
        swprintf_s(buf, L"%ld", value);
        ListView_SetItemText(hList, row, column, buf);
    }
}

MyTable::MyTable()
    : m_hDlg(nullptr), m_hList(nullptr), m_hDelete(nullptr),
      m_pListener(nullptr), m_updating(false) {
}

MyTable::~MyTable() {
    Close();
}

bool MyTable::Open(HWND hParent, HINSTANCE hInstance) {
    if (m_hDlg) {
        ShowWindow(m_hDlg, SW_SHOW);
        SetForegroundWindow(m_hDlg);
        return true;
    }

    INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX), ICC_LISTVIEW_CLASSES };
    InitCommonControlsEx(&icc);

    alignas(4) BYTE buffer[128] = {};
    DLGTEMPLATE* pTemplate = reinterpret_cast<DLGTEMPLATE*>(buffer);
    pTemplate->style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
    pTemplate->cdit = 0;
    pTemplate->x = 20;
    pTemplate->y = 20;
    pTemplate->cx = 260;
    pTemplate->cy = 170;

    WORD* pWord = reinterpret_cast<WORD*>(pTemplate + 1);
    *pWord++ = 0;
    *pWord++ = 0;
    const wchar_t title[] = L"Таблиця об'єктів";
    std::memcpy(pWord, title, sizeof(title));

    HWND hDlg = CreateDialogIndirectParam(hInstance, pTemplate, hParent,
        DlgProc, reinterpret_cast<LPARAM>(this));
    if (!hDlg) return false;

    ShowWindow(hDlg, SW_SHOW);
    return true;
}

void MyTable::Close() {
    if (m_hDlg) {
        DestroyWindow(m_hDlg);
    }
}

bool MyTable::ProcessMessage(MSG* pMsg) {
    return m_hDlg != nullptr && IsDialogMessage(m_hDlg, pMsg) != FALSE;
}

INT_PTR CALLBACK MyTable::DlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp) {
    MyTable* pTable = reinterpret_cast<MyTable*>(GetWindowLongPtr(hDlg, DWLP_USER));

    if (msg == WM_INITDIALOG) {
        pTable = reinterpret_cast<MyTable*>(lp);
        SetWindowLongPtr(hDlg, DWLP_USER, reinterpret_cast<LONG_PTR>(pTable));
        return pTable->OnInitDialog(hDlg) ? TRUE : FALSE;
    }
    if (!pTable) return FALSE;

    switch (msg) {
    case WM_SIZE:
        pTable->OnSize();
        return TRUE;
    case WM_COMMAND:
        pTable->OnCommand(LOWORD(wp), HIWORD(wp));
        return TRUE;
    case WM_NOTIFY:
        pTable->OnNotify(lp);
        return TRUE;
    case WM_CLOSE:
        pTable->OnClose();
        return TRUE;
    case WM_DESTROY:
        pTable->OnDestroy();
        return TRUE;
    }
    return FALSE;
}

bool MyTable::OnInitDialog(HWND hDlg) {
    m_hDlg = hDlg;
    HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hDlg, GWLP_HINSTANCE));
    HFONT hFont = reinterpret_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));

    m_hList = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, L"",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS,
        0, 0, 0, 0, hDlg, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDC_TABLE_LIST)),
        hInstance, nullptr);

    m_hDelete = CreateWindowEx(0, L"BUTTON", L"Видалити вибраний",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
        0, 0, 0, 0, hDlg, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDC_TABLE_DELETE)),
        hInstance, nullptr);

    if (!m_hList || !m_hDelete) return false;

    SendMessage(m_hList, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
    SendMessage(m_hDelete, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
    ListView_SetExtendedListViewStyle(m_hList, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    AddColumn(m_hList, 0, L"№", 40);
    AddColumn(m_hList, 1, L"Тип", 150);
    AddColumn(m_hList, 2, L"X1", 55);
    AddColumn(m_hList, 3, L"Y1", 55);
    AddColumn(m_hList, 4, L"X2", 55);
    AddColumn(m_hList, 5, L"Y2", 55);

    OnSize();
    return true;
}

void MyTable::OnSize() {
    if (!m_hList || !m_hDelete) return;

    RECT rc;
    GetClientRect(m_hDlg, &rc);
    const int bar = 38;
    int listHeight = rc.bottom - bar;
    if (listHeight < 0) listHeight = 0;

    MoveWindow(m_hList, 0, 0, rc.right, listHeight, TRUE);
    MoveWindow(m_hDelete, 8, listHeight + 7, 160, 24, TRUE);
}

void MyTable::OnCommand(WORD id, WORD code) {
    if (id == IDC_TABLE_DELETE && code == BN_CLICKED) {
        RequestDelete();
    }
    else if (id == IDCANCEL) {
        OnClose();
    }
}

void MyTable::OnNotify(LPARAM lp) {
    LPNMHDR pHdr = reinterpret_cast<LPNMHDR>(lp);
    if (pHdr->idFrom != IDC_TABLE_LIST) return;

    if (pHdr->code == LVN_ITEMCHANGED) {
        LPNMLISTVIEW pView = reinterpret_cast<LPNMLISTVIEW>(lp);
        bool nowSelected = (pView->uNewState & LVIS_SELECTED) != 0;
        bool wasSelected = (pView->uOldState & LVIS_SELECTED) != 0;
        if (!m_updating && (pView->uChanged & LVIF_STATE) && nowSelected && !wasSelected && m_pListener) {
            m_pListener->OnRowSelected(pView->iItem);
        }
    }
    else if (pHdr->code == LVN_KEYDOWN) {
        LPNMLVKEYDOWN pKey = reinterpret_cast<LPNMLVKEYDOWN>(lp);
        if (pKey->wVKey == VK_DELETE) {
            RequestDelete();
        }
    }
}

void MyTable::OnClose() {
    DestroyWindow(m_hDlg);
}

void MyTable::OnDestroy() {
    m_hDlg = nullptr;
    m_hList = nullptr;
    m_hDelete = nullptr;
    if (m_pListener) {
        m_pListener->OnTableClosed();
    }
}

int MyTable::SelectedRow() const {
    if (!m_hList) return -1;
    return ListView_GetNextItem(m_hList, -1, LVNI_SELECTED);
}

void MyTable::RequestDelete() {
    int row = SelectedRow();
    if (row >= 0 && m_pListener) {
        m_pListener->OnRowDeleteRequested(row);
    }
}

void MyTable::Clear() {
    if (!m_hList) return;
    m_updating = true;
    ListView_DeleteAllItems(m_hList);
    m_updating = false;
}

void MyTable::AddRow(const wchar_t* typeName, long x1, long y1, long x2, long y2) {
    if (!m_hList) return;
    m_updating = true;

    int row = ListView_GetItemCount(m_hList);
    wchar_t number[32];
    swprintf_s(number, L"%d", row + 1);

    LVITEM item = {};
    item.mask = LVIF_TEXT;
    item.iItem = row;
    item.iSubItem = 0;
    item.pszText = number;
    ListView_InsertItem(m_hList, &item);

    ListView_SetItemText(m_hList, row, 1, const_cast<LPWSTR>(typeName));
    SetCellNumber(m_hList, row, 2, x1);
    SetCellNumber(m_hList, row, 3, y1);
    SetCellNumber(m_hList, row, 4, x2);
    SetCellNumber(m_hList, row, 5, y2);

    m_updating = false;
}

void MyTable::RemoveRow(int rowIndex) {
    if (!m_hList) return;
    m_updating = true;
    ListView_DeleteItem(m_hList, rowIndex);
    Renumber(rowIndex);
    m_updating = false;
}

void MyTable::Renumber(int fromRow) {
    int count = ListView_GetItemCount(m_hList);
    for (int i = fromRow; i < count; ++i) {
        SetCellNumber(m_hList, i, 0, i + 1);
    }
}

void MyTable::SelectRow(int rowIndex) {
    if (!m_hList) return;
    m_updating = true;
    if (rowIndex < 0) {
        ListView_SetItemState(m_hList, -1, 0, LVIS_SELECTED | LVIS_FOCUSED);
    }
    else {
        ListView_SetItemState(m_hList, rowIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
        ListView_EnsureVisible(m_hList, rowIndex, FALSE);
    }
    m_updating = false;
}
