#include "ShapeEditor.h"
#include "PointShape.h"
#include "LineShape.h"
#include "RectShape.h"
#include "EllipseShape.h"

ShapeEditor::ShapeEditor()
    : m_currentTool(TOOL_POINT), m_isDrawing(false), m_pTrace(nullptr) {
    m_ptStart = { 0, 0 };
    m_ptEnd = { 0, 0 };
}

ShapeEditor::~ShapeEditor() {
    delete m_pTrace;
}

const wchar_t* ShapeEditor::ToolName(CurrentTool tool) {
    switch (tool) {
    case TOOL_POINT:   return L"Крапка";
    case TOOL_LINE:    return L"Лінія";
    case TOOL_RECT:    return L"Прямокутник";
    case TOOL_ELLIPSE: return L"Еліпс";
    }
    return L"";
}

Shape* ShapeEditor::CreateShape(CurrentTool tool) const {
    switch (tool) {
    case TOOL_POINT:   return new PointShape();
    case TOOL_LINE:    return new LineShape();
    case TOOL_RECT:    return new RectShape();
    case TOOL_ELLIPSE: return new EllipseShape();
    }
    return nullptr;
}

void ShapeEditor::DrawTrace(HWND hWnd) const {
    if (!m_pTrace) return;

    HDC hdc = GetDC(hWnd);
    int oldRop = SetROP2(hdc, R2_NOTXORPEN);
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    m_pTrace->Trace(hdc); 

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    SetROP2(hdc, oldRop);
    ReleaseDC(hWnd, hdc);
}

void ShapeEditor::OnLButtonDown(HWND hWnd, int x, int y) {
    if (m_isDrawing) return;

    m_pTrace = CreateShape(m_currentTool);
    if (!m_pTrace) return;

    m_ptStart.x = x; m_ptStart.y = y;
    m_ptEnd = m_ptStart;
    m_pTrace->SetCoord(x, y, x, y);
    m_isDrawing = true;
    SetCapture(hWnd);
}

void ShapeEditor::OnMouseMove(HWND hWnd, int x, int y) {
    if (!m_isDrawing || !m_pTrace) return;

    DrawTrace(hWnd);                         
    m_ptEnd.x = x; m_ptEnd.y = y;
    m_pTrace->SetCoord(m_ptStart.x, m_ptStart.y, m_ptEnd.x, m_ptEnd.y);
    DrawTrace(hWnd);                         
}

Shape* ShapeEditor::OnLButtonUp(HWND hWnd, int x, int y) {
    if (!m_isDrawing || !m_pTrace) return nullptr;

    DrawTrace(hWnd);                         
    m_ptEnd.x = x; m_ptEnd.y = y;
    m_pTrace->SetCoord(m_ptStart.x, m_ptStart.y, m_ptEnd.x, m_ptEnd.y);

    Shape* pShape = m_pTrace;                
    m_pTrace = nullptr;
    m_isDrawing = false;
    ReleaseCapture();
    return pShape;
}

void ShapeEditor::OnCaptureLost(HWND hWnd) {
    if (!m_isDrawing) return;

    DrawTrace(hWnd);
    delete m_pTrace;
    m_pTrace = nullptr;
    m_isDrawing = false;
}
