#include "Toolbar.h"
#include "resource.h"
#include <cstdlib>
#include <cwchar>

#pragma comment(lib, "comctl32.lib")

namespace {
    struct ButtonInfo {
        int imageIndex;
        int commandId;
        const wchar_t* tooltip;
    };

    const ButtonInfo kButtons[] = {
        { 0, IDM_OBJECT_POINT,   L"Крапка" },
        { 1, IDM_OBJECT_LINE,    L"Лінія" },
        { 2, IDM_OBJECT_RECT,    L"Прямокутник (від центру до кута)" },
        { 3, IDM_OBJECT_ELLIPSE, L"Еліпс (по двох протилежних кутах)" },
    };
    const int kButtonCount = sizeof(kButtons) / sizeof(kButtons[0]);
    const int kImageSize = 24;
    const COLORREF kMaskColor = RGB(255, 0, 255);   
}

Toolbar::Toolbar() : m_hWnd(nullptr), m_hImageList(nullptr) {}

Toolbar::~Toolbar() {
    if (m_hImageList) ImageList_Destroy(m_hImageList);
}

const wchar_t* Toolbar::TooltipText(int commandId) {
    for (int i = 0; i < kButtonCount; ++i) {
        if (kButtons[i].commandId == commandId) return kButtons[i].tooltip;
    }
    return nullptr;
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

    HBITMAP hBitmap = (HBITMAP)LoadImage(hInstance, MAKEINTRESOURCE(IDB_TOOLBAR),
        IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if (!hBitmap) return false;

    m_hImageList = ImageList_Create(kImageSize, kImageSize, ILC_COLOR24 | ILC_MASK, kButtonCount, 0);
    ImageList_AddMasked(m_hImageList, hBitmap, kMaskColor);
    DeleteObject(hBitmap);
    SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)m_hImageList);

    TBBUTTON buttons[kButtonCount];
    for (int i = 0; i < kButtonCount; ++i) {
        buttons[i] = TBBUTTON();                   
        buttons[i].iBitmap = kButtons[i].imageIndex;
        buttons[i].idCommand = kButtons[i].commandId;
        buttons[i].fsState = TBSTATE_ENABLED;
        buttons[i].fsStyle = TBSTYLE_CHECKGROUP;   
    }
    SendMessage(m_hWnd, TB_ADDBUTTONS, kButtonCount, (LPARAM)buttons);
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

    const wchar_t* text = TooltipText((int)pnmh->idFrom);
    if (!text) return false;

    LPNMTTDISPINFO pInfo = (LPNMTTDISPINFO)pnmh;
    wcsncpy_s(pInfo->szText, _countof(pInfo->szText), text, _TRUNCATE);
    return true;
}
