#include "MyEditor.h"
#include "Shape.h"
#include "ToolRegistry.h"

MyEditor::MyEditor()
    : pcshape(nullptr), m_shapeCount(0), m_currentTool(0),
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

    if (m_shapeCount < N) {
        pcshape[m_shapeCount++] = pShape;
        return EDIT_ADDED;
    }
    delete pShape;                           
    return EDIT_FULL;
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
}
