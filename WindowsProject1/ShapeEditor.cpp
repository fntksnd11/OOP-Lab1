#include "ShapeEditor.h"
#include "Application.h"
#include "PointShape.h"
#include "LineShape.h"
#include "RectShape.h"
#include "EllipseShape.h"
#include <cmath>

ShapeEditor::ShapeEditor()
    : m_currentTool(TOOL_POINT), m_isDrawing(false) {
    m_ptStart = { 0, 0 };
    m_ptEnd = { 0, 0 };
}

void ShapeEditor::OnLButtonDown(HWND hWnd, LPARAM lParam) {
    m_isDrawing = true;
    m_ptStart.x = LOWORD(lParam);
    m_ptStart.y = HIWORD(lParam);
    m_ptEnd = m_ptStart;
    SetCapture(hWnd);
}

void ShapeEditor::DrawRubberBand(HWND hWnd, POINT ptStart, POINT ptEnd) {
    HDC hdc = GetDC(hWnd);
    int oldROP = SetROP2(hdc, R2_NOTXORPEN); 
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    switch (m_currentTool) {
    case TOOL_LINE:
        MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
        LineTo(hdc, ptEnd.x, ptEnd.y);
        break;
    case TOOL_RECT:
        Rectangle(hdc, min(ptStart.x, ptEnd.x), min(ptStart.y, ptEnd.y),
            max(ptStart.x, ptEnd.x), max(ptStart.y, ptEnd.y));
        break;
    case TOOL_ELLIPSE: {
        long rx = std::abs(ptEnd.x - ptStart.x);
        long ry = std::abs(ptEnd.y - ptStart.y);
        Ellipse(hdc, ptStart.x - rx, ptStart.y - ry, ptStart.x + rx, ptStart.y + ry);
        break;
    }
    default:
        break;
    }

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    SetROP2(hdc, oldROP);
    ReleaseDC(hWnd, hdc);
}

void ShapeEditor::OnMouseMove(HWND hWnd, LPARAM lParam) {
    if (!m_isDrawing || m_currentTool == TOOL_POINT) return;

    DrawRubberBand(hWnd, m_ptStart, m_ptEnd);

    m_ptEnd.x = LOWORD(lParam);
    m_ptEnd.y = HIWORD(lParam);


    DrawRubberBand(hWnd, m_ptStart, m_ptEnd);
}

void ShapeEditor::OnLButtonUp(HWND hWnd, LPARAM lParam, Application* pApp) {
    if (!m_isDrawing) return;

    if (m_currentTool != TOOL_POINT) {

        DrawRubberBand(hWnd, m_ptStart, m_ptEnd);
    }

    m_ptEnd.x = LOWORD(lParam);
    m_ptEnd.y = HIWORD(lParam);

    Shape* pShape = nullptr;
    switch (m_currentTool) {
    case TOOL_POINT:   pShape = new PointShape(); break;
    case TOOL_LINE:    pShape = new LineShape(); break;
    case TOOL_RECT:    pShape = new RectShape(); break;
    case TOOL_ELLIPSE: pShape = new EllipseShape(); break;
    }

    if (pShape) {
        pShape->SetCoord(m_ptStart.x, m_ptStart.y, m_ptEnd.x, m_ptEnd.y);
        pApp->AddShape(pShape);
    }

    m_isDrawing = false;
    ReleaseCapture();
    InvalidateRect(hWnd, NULL, TRUE);
}