#pragma once
#include <windows.h>

class Shape;

const int N = 109;

enum EditResult {
    EDIT_NONE,
    EDIT_ADDED,
    EDIT_FULL
};

class MyEditor {
private:
    static MyEditor* s_pInstance;

    Shape** pcshape;
    int     m_shapeCount;
    int     m_selected;

    int     m_currentTool;
    bool    m_isDrawing;
    POINT   m_ptStart;
    POINT   m_ptEnd;
    Shape*  m_pTrace;

    MyEditor();
    ~MyEditor();
    MyEditor(const MyEditor&) = delete;
    MyEditor& operator=(const MyEditor&) = delete;

    void DrawTrace(HWND hWnd) const;

public:
    static MyEditor* GetInstance();
    static void      DestroyInstance();

    void SetTool(int toolIndex) { m_currentTool = toolIndex; }
    int  GetTool() const { return m_currentTool; }

    void       OnLButtonDown(HWND hWnd, int x, int y);
    void       OnMouseMove(HWND hWnd, int x, int y);
    EditResult OnLButtonUp(HWND hWnd, int x, int y);
    void       OnCaptureLost(HWND hWnd);

    void OnPaint(HDC hdc);

    int          GetCount() const { return m_shapeCount; }
    const Shape* GetShape(int index) const;
    bool         AddShape(Shape* pShape);
    void         RemoveShape(int index);
    void         Clear();

    void SetSelected(int index) { m_selected = index; }
    int  GetSelected() const { return m_selected; }
};
