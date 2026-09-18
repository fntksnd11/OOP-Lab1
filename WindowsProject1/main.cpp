#include "Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Application app(hInstance);
    if (!app.Initialize()) {
        return -1;
    }
    return app.Run();
}