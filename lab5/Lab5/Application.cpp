#include "Application.h"
#include "ToolRegistry.h"
#include "Shape.h"
#include "ShapeFile.h"
#include "resource.h"
#include <windowsx.h>
#include <commctrl.h>
#include <commdlg.h>
#include <string>

#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comdlg32.lib")

namespace {
    const wchar_t* const kClassName = L"Lab5_EditorClass";
    const wchar_t* const kTitleBase = L"Лабораторна робота №5 - Графічний редактор";

    const wchar_t kFileFilter[] = L"Файли об'єктів (*.txt)\0*.txt\0Усі файли (*.*)\0*.*\0";
}

Application::Application(HINSTANCE hInstance)
    : m_hInstance(hInstance), m_hWnd(nullptr), m_pEditor(nullptr) {
    m_pEditor = MyEditor::GetInstance();
    m_table.SetListener(this);
}

Application::~Application() {
    m_table.SetListener(nullptr);
    MyEditor::DestroyInstance();
    m_pEditor = nullptr;
}

bool Application::Initialize() {
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Application::WindowProc;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
    wcex.lpszClassName = kClassName;

    if (!RegisterClassEx(&wcex)) return false;

    m_hWnd = CreateWindow(
        kClassName, kTitleBase,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, 0, 900, 600,
        nullptr, nullptr, m_hInstance, this);

    if (!m_hWnd) return false;

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
    return true;
}

int Application::Run() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (m_table.ProcessMessage(&msg)) continue;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK Application::WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    Application* pApp = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (msg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lp);
        pApp = reinterpret_cast<Application*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));
        pApp->m_hWnd = hWnd;
    }

    if (!pApp) return DefWindowProc(hWnd, msg, wp, lp);
    return pApp->HandleMessage(hWnd, msg, wp, lp);
}

LRESULT Application::HandleMessage(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        return OnCreate() ? 0 : -1;

    case WM_SIZE:
        OnSize();
        return 0;

    case WM_COMMAND:
        OnCommand(LOWORD(wp));
        return 0;

    case WM_NOTIFY:
        return OnNotify(lp);

    case WM_INITMENUPOPUP:
        OnInitMenuPopup(reinterpret_cast<HMENU>(wp));
        return 0;

    case WM_PAINT:
        OnPaint();
        return 0;

    case WM_LBUTTONDOWN:
        OnLButtonDown(lp);
        return 0;

    case WM_MOUSEMOVE:
        OnMouseMove(lp);
        return 0;

    case WM_LBUTTONUP:
        OnLButtonUp(lp);
        return 0;

    case WM_CAPTURECHANGED:
        OnCaptureChanged();
        return 0;

    case WM_DESTROY:
        OnDestroy();
        return 0;
    }
    return DefWindowProc(hWnd, msg, wp, lp);
}

bool Application::OnCreate() {
    BuildObjectsMenu();
    if (!m_toolbar.Create(m_hWnd, m_hInstance)) return false;
    SetTool(0);
    return true;
}

void Application::OnDestroy() {
    m_table.Close();
    PostQuitMessage(0);
}

void Application::OnSize() {
    m_toolbar.AutoSize();
}

void Application::OnCommand(WORD id) {
    switch (id) {
    case IDM_FILE_EXIT:
        DestroyWindow(m_hWnd);
        return;
    case IDM_FILE_OPEN:
        OnFileOpen();
        return;
    case IDM_FILE_SAVE:
        OnFileSave();
        return;
    case IDM_WINDOW_TABLE:
        ToggleTable();
        return;
    case IDM_HELP_ABOUT:
        MessageBox(m_hWnd,
            L"Лабораторна робота №5\nГрафічний редактор: Singleton, таблиця об'єктів,\nзапис і завантаження файлу",
            L"Про програму", MB_OK | MB_ICONINFORMATION);
        return;
    }

    int index = ToolRegistry::IndexByCommand(id);
    if (index >= 0) SetTool(index);
}

LRESULT Application::OnNotify(LPARAM lp) {
    m_toolbar.OnNotify(reinterpret_cast<LPNMHDR>(lp));
    return 0;
}

void Application::OnInitMenuPopup(HMENU hMenu) {
    int current = m_pEditor->GetTool();
    for (int i = 0; i < ToolRegistry::Count(); ++i) {
        CheckMenuItem(hMenu, ToolRegistry::CommandId(i),
            MF_BYCOMMAND | (i == current ? MF_CHECKED : MF_UNCHECKED));
    }
    CheckMenuItem(hMenu, IDM_WINDOW_TABLE,
        MF_BYCOMMAND | (m_table.IsOpen() ? MF_CHECKED : MF_UNCHECKED));
}

void Application::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);
    m_pEditor->OnPaint(hdc);
    EndPaint(m_hWnd, &ps);
}

void Application::OnLButtonDown(LPARAM lp) {
    m_pEditor->OnLButtonDown(m_hWnd, GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
}

void Application::OnMouseMove(LPARAM lp) {
    m_pEditor->OnMouseMove(m_hWnd, GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
}

void Application::OnLButtonUp(LPARAM lp) {
    EditResult result = m_pEditor->OnLButtonUp(m_hWnd, GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
    if (result == EDIT_ADDED) {
        AddTableRow(m_pEditor->GetShape(m_pEditor->GetCount() - 1));
        InvalidateRect(m_hWnd, NULL, TRUE);
    }
    else if (result == EDIT_FULL) {
        MessageBox(m_hWnd, L"Масив заповнено: досягнуто максимальної кількості об'єктів (109).",
            L"Графічний редактор", MB_OK | MB_ICONINFORMATION);
    }
}

void Application::OnCaptureChanged() {
    m_pEditor->OnCaptureLost(m_hWnd);
}

void Application::OnFileSave() {
    wchar_t path[MAX_PATH] = L"shapes.txt";

    OPENFILENAME ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = kFileFilter;
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrDefExt = L"txt";
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

    if (!GetSaveFileName(&ofn)) return;

    if (!ShapeFile::Save(path, *m_pEditor)) {
        MessageBox(m_hWnd, L"Не вдалося записати файл.", L"Помилка", MB_OK | MB_ICONERROR);
    }
}

void Application::OnFileOpen() {
    wchar_t path[MAX_PATH] = L"";

    OPENFILENAME ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = kFileFilter;
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

    if (!GetOpenFileName(&ofn)) return;

    if (ShapeFile::Load(path, *m_pEditor) < 0) {
        MessageBox(m_hWnd, L"Не вдалося прочитати файл: невірний формат.", L"Помилка", MB_OK | MB_ICONERROR);
        return;
    }
    FillTable();
    InvalidateRect(m_hWnd, NULL, TRUE);
}

void Application::ToggleTable() {
    if (m_table.IsOpen()) {
        m_table.Close();
        return;
    }
    if (!m_table.Open(m_hWnd, m_hInstance)) {
        MessageBox(m_hWnd, L"Не вдалося відкрити вікно таблиці.", L"Помилка", MB_OK | MB_ICONERROR);
        return;
    }
    FillTable();
}

void Application::FillTable() {
    if (!m_table.IsOpen()) return;

    m_table.Clear();
    for (int i = 0; i < m_pEditor->GetCount(); ++i) {
        AddTableRow(m_pEditor->GetShape(i));
    }
}

void Application::AddTableRow(const Shape* pShape) {
    if (!m_table.IsOpen() || !pShape) return;

    int kind = pShape->GetKind();
    if (kind < 0 || kind >= ToolRegistry::Count()) return;

    m_table.AddRow(ToolRegistry::Get(kind).name,
        pShape->X1(), pShape->Y1(), pShape->X2(), pShape->Y2());
}

void Application::OnRowSelected(int rowIndex) {
    m_pEditor->SetSelected(rowIndex);
    InvalidateRect(m_hWnd, NULL, TRUE);
}

void Application::OnRowDeleteRequested(int rowIndex) {
    m_pEditor->RemoveShape(rowIndex);
    m_table.RemoveRow(rowIndex);
    InvalidateRect(m_hWnd, NULL, TRUE);
}

void Application::OnTableClosed() {
    m_pEditor->SetSelected(-1);
    if (m_hWnd) InvalidateRect(m_hWnd, NULL, TRUE);
}

void Application::BuildObjectsMenu() {
    HMENU hMenu = GetMenu(m_hWnd);
    HMENU hObjects = hMenu ? GetSubMenu(hMenu, 1) : nullptr;
    if (!hObjects) return;

    DeleteMenu(hObjects, 0, MF_BYPOSITION);
    for (int i = 0; i < ToolRegistry::Count(); ++i) {
        AppendMenu(hObjects, MF_STRING, ToolRegistry::CommandId(i), ToolRegistry::Get(i).name);
    }
}

void Application::SetTool(int toolIndex) {
    m_pEditor->SetTool(toolIndex);
    m_toolbar.CheckTool(ToolRegistry::CommandId(toolIndex));
    UpdateTitle();
}

void Application::UpdateTitle() {
    std::wstring title = kTitleBase;
    title += L" - Об'єкт: ";
    title += ToolRegistry::Get(m_pEditor->GetTool()).name;
    SetWindowText(m_hWnd, title.c_str());
}
