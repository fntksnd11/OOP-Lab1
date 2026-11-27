#include "MyEditor.h"
#include "Shape.h"
#include "ToolRegistry.h"

MyEditor* MyEditor::s_pInstance = nullptr;

MyEditor* MyEditor::GetInstance() {
    if (!s_pInstance) {
        s_pInstance = new MyEditor();
    }
    return s_pInstance;
}

void MyEditor::DestroyInstance() {
    delete s_pInstance;
    s_pInstance = nullptr;
}

MyEditor::MyEditor()
    : pcshape(nullptr), m_shapeCount(0), m_selected(-1), m_currentTool(0),
      m_isDrawing(false), m_pTrace(nullptr) {
    m_ptStart = { 0, 0 };
    m_ptEnd = { 0, 0 };

    pcshape = new Shape*[N];
    for (int i = 0; i < N; ++i) {
        pcshape[i] = nullptr;
    }
}

MyEditor::~MyEditor() {
    delete m_pTrace;
    for (int i = 0; i < m_shapeCount; ++i) {
        delete pcshape[i];
    }
    delete[] pcshape;
}

const Shape* MyEditor::GetShape(int index) const {
    return (index >= 0 && index < m_shapeCount) ? pcshape[index] : nullptr;
}

bool MyEditor::AddShape(Shape* pShape) {
    if (m_shapeCount < N) {
        pcshape[m_shapeCount++] = pShape;
        return true;
    }
    delete pShape;
    return false;
}

void MyEditor::RemoveShape(int index) {
    if (index < 0 || index >= m_shapeCount) return;

    delete pcshape[index];
    for (int i = index; i < m_shapeCount - 1; ++i) {
        pcshape[i] = pcshape[i + 1];
    }
    pcshape[--m_shapeCount] = nullptr;

    if (m_selected == index) m_selected = -1;
    else if (m_selected > index) --m_selected;
}

void MyEditor::Clear() {
    for (int i = 0; i < m_shapeCount; ++i) {
        delete pcshape[i];
        pcshape[i] = nullptr;
    }
    m_shapeCount = 0;
    m_selected = -1;
}

void MyEditor::DrawTrace(HWND hWnd) const {
    if (!m_pTrace) return;

    HDC hdc = GetDC(hWnd);
    int oldRop = SetROP2(hdc, R2_NOTXORPEN);
    int oldBk = SetBkMode(hdc, TRANSPARENT);
    HPEN hPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    m_pTrace->Trace(hdc);

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    SetBkMode(hdc, oldBk);
    SetROP2(hdc, oldRop);
    ReleaseDC(hWnd, hdc);
}

void MyEditor::OnLButtonDown(HWND hWnd, int x, int y) {
    if (m_isDrawing) return;

    m_pTrace = ToolRegistry::Get(m_currentTool).create();
    if (!m_pTrace) return;

    m_pTrace->SetKind(m_currentTool);
    m_ptStart.x = x; m_ptStart.y = y;
    m_ptEnd = m_ptStart;
    m_pTrace->SetCoord(x, y, x, y);
    m_isDrawing = true;
    SetCapture(hWnd);
}

void MyEditor::OnMouseMove(HWND hWnd, int x, int y) {
    if (!m_isDrawing || !m_pTrace) return;

    DrawTrace(hWnd);
    m_ptEnd.x = x; m_ptEnd.y = y;
    m_pTrace->SetCoord(m_ptStart.x, m_ptStart.y, m_ptEnd.x, m_ptEnd.y);
    DrawTrace(hWnd);
}

EditResult MyEditor::OnLButtonUp(HWND hWnd, int x, int y) {
    if (!m_isDrawing || !m_pTrace) return EDIT_NONE;

    DrawTrace(hWnd);
    m_ptEnd.x = x; m_ptEnd.y = y;
    m_pTrace->SetCoord(m_ptStart.x, m_ptStart.y, m_ptEnd.x, m_ptEnd.y);

    Shape* pShape = m_pTrace;
    m_pTrace = nullptr;
    m_isDrawing = false;
    ReleaseCapture();

    return AddShape(pShape) ? EDIT_ADDED : EDIT_FULL;
}

void MyEditor::OnCaptureLost(HWND hWnd) {
    if (!m_isDrawing) return;

    DrawTrace(hWnd);
    delete m_pTrace;
    m_pTrace = nullptr;
    m_isDrawing = false;
}

void MyEditor::OnPaint(HDC hdc) {
    for (int i = 0; i < m_shapeCount; ++i) {
        pcshape[i]->Show(hdc);
    }

    if (m_selected >= 0 && m_selected < m_shapeCount) {
        HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

        pcshape[m_selected]->Highlight(hdc);

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
    }
}
