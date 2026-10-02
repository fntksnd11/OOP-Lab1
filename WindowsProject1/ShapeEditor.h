#pragma once
#include <windows.h>

enum CurrentTool {
    TOOL_POINT,
    TOOL_LINE,
    TOOL_RECT,
    TOOL_ELLIPSE
};

class ShapeEditor {
private:
    CurrentTool m_currentTool;
    bool m_isDrawing;
    POINT m_ptStart;
    POINT m_ptEnd;

public:
    ShapeEditor();

    void SetTool(CurrentTool tool) { m_currentTool = tool; }
    CurrentTool GetTool() const { return m_currentTool; }

    void OnLButtonDown(HWND hWnd, LPARAM lParam);
    void OnMouseMove(HWND hWnd, LPARAM lParam);
    void OnLButtonUp(HWND hWnd, LPARAM lParam, class Application* pApp);

    void DrawRubberBand(HWND hWnd, POINT ptStart, POINT ptEnd);
};