#include "Toolbar.h"
#include "ToolRegistry.h"
#include "resource.h"
#include <vector>
#include <cstdlib>
#include <cwchar>

#pragma comment(lib, "comctl32.lib")

namespace {
    const int kImageSize = 24;
    const COLORREF kMaskColor = RGB(255, 0, 255);
}

Toolbar::Toolbar() : m_hWnd(nullptr), m_hImageList(nullptr) {}

Toolbar::~Toolbar() {
    if (m_hImageList) ImageList_Destroy(m_hImageList);
}

bool Toolbar::Create(HWND hParent, HINSTANCE hInstance) {
    INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX), ICC_BAR_CLASSES };
    InitCommonControlsEx(&icc);

    m_hWnd = CreateWindowEx(
        0, TOOLBARCLASSNAME, nullptr,
        WS_CHILD | WS_VISIBLE | CCS_TOP | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        0, 0, 0, 0,
        hParent, (HMENU)(INT_PTR)IDC_TOOLBAR, hInstance, nullptr);
    if (!m_hWnd) return false;

    SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);

    const int count = ToolRegistry::Count();

    HBITMAP hBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_TOOLBAR),
        IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if (!hBitmap) return false;

    m_hImageList = ImageList_Create(kImageSize, kImageSize, ILC_COLOR24 | ILC_MASK, count, 0);
    ImageList_AddMasked(m_hImageList, hBitmap, kMaskColor);
    DeleteObject(hBitmap);
    SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)m_hImageList);

    std::vector<TBBUTTON> buttons(count);
    for (int i = 0; i < count; ++i) {
        buttons[i] = TBBUTTON();
        buttons[i].iBitmap = i;
        buttons[i].idCommand = ToolRegistry::CommandId(i);
        buttons[i].fsState = TBSTATE_ENABLED;
        buttons[i].fsStyle = TBSTYLE_CHECKGROUP;
    }
    SendMessage(m_hWnd, TB_ADDBUTTONS, count, (LPARAM)buttons.data());
    SendMessage(m_hWnd, TB_AUTOSIZE, 0, 0);
    return true;
}

void Toolbar::AutoSize() {
    if (m_hWnd) SendMessage(m_hWnd, TB_AUTOSIZE, 0, 0);
}

void Toolbar::CheckTool(int commandId) {
    if (m_hWnd) SendMessage(m_hWnd, TB_CHECKBUTTON, commandId, MAKELONG(TRUE, 0));
}

bool Toolbar::OnNotify(LPNMHDR pnmh) {
    if (!pnmh || pnmh->code != TTN_GETDISPINFO) return false;

    int index = ToolRegistry::IndexByCommand((int)pnmh->idFrom);
    if (index < 0) return false;

    LPNMTTDISPINFO pInfo = (LPNMTTDISPINFO)pnmh;
    wcsncpy_s(pInfo->szText, _countof(pInfo->szText),
        ToolRegistry::Get(index).tooltip, _TRUNCATE);
    return true;
}
