#pragma once
#include <windows.h>

class Shape;

// Ж = 9 (як у Lab3)  =>  N = Ж + 100 = 109
const int N = 109;

// Результат завершення вводу фігури
enum EditResult {
    EDIT_NONE,     // нічого не додано
    EDIT_ADDED,    // фігуру додано в масив - потрібна перерисовка
    EDIT_FULL      // масив заповнений, фігуру відкинуто
};

// Редактор об'єктів: зберігає масив фігур, керує вводом мишею,
// "гумовим" слідом та відображенням. Про вікно програми нічого не знає.
// Варіант (парний номер): об'єкт створюється ДИНАМІЧНО (new) та
// знищується (delete) в Application.
class MyEditor {
private:
    Shape** pcshape;        // динамічний масив вказівників на фігури
    int     m_shapeCount;

    int     m_currentTool;  // індекс інструмента в ToolRegistry
    bool    m_isDrawing;
    POINT   m_ptStart;
    POINT   m_ptEnd;
    Shape*  m_pTrace;       // фігура, що зараз вводиться

    void DrawTrace(HWND hWnd) const;

public:
    MyEditor();
    ~MyEditor();
    MyEditor(const MyEditor&) = delete;
    MyEditor& operator=(const MyEditor&) = delete;

    void SetTool(int toolIndex) { m_currentTool = toolIndex; }
    int  GetTool() const { return m_currentTool; }

    void       OnLButtonDown(HWND hWnd, int x, int y);
    void       OnMouseMove(HWND hWnd, int x, int y);
    EditResult OnLButtonUp(HWND hWnd, int x, int y);
    void       OnCaptureLost(HWND hWnd);

    void OnPaint(HDC hdc);   // відобразити всі фігури
};
