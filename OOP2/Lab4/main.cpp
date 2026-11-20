#include "Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    Application app(hInstance);
    if (!app.Initialize()) {
        return -1;
    }
    return app.Run();
}
