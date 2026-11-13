#pragma once
#include <windows.h>

class Shape;

enum CurrentTool {
    TOOL_POINT,
    TOOL_LINE,
    TOOL_RECT,
    TOOL_ELLIPSE
};

class ShapeEditor {
private:
    CurrentTool m_currentTool;
    bool        m_isDrawing;
    POINT       m_ptStart;
    POINT       m_ptEnd;
    Shape*      m_pTrace;   

    Shape* CreateShape(CurrentTool tool) const;
    void   DrawTrace(HWND hWnd) const;

public:
    ShapeEditor();
    ~ShapeEditor();
    ShapeEditor(const ShapeEditor&) = delete;
    ShapeEditor& operator=(const ShapeEditor&) = delete;

    void        SetTool(CurrentTool tool) { m_currentTool = tool; }
    CurrentTool GetTool() const { return m_currentTool; }
    static const wchar_t* ToolName(CurrentTool tool);

    void   OnLButtonDown(HWND hWnd, int x, int y);
    void   OnMouseMove(HWND hWnd, int x, int y);
  
    Shape* OnLButtonUp(HWND hWnd, int x, int y);
   
    void   OnCaptureLost(HWND hWnd);
};
