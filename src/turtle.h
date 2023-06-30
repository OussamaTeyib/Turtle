#ifndef TURTLE_H
#define TURTLE_H

#include <windows.h>

void die(char *msg)
{
    MessageBox(NULL, msg, "ERROR", MB_ICONEXCLAMATION | MB_OK);    
    exit(EXIT_FAILURE);
}

typedef struct {
    HWND hwnd;
    HDC hdc;
} Turtle;

POINT GetCentre(HWND hwnd)
{
    POINT centre;
    RECT rect;

    GetClientRect(hwnd, &rect);
    centre.x = rect.right / 2;
    centre.y = rect.bottom / 2;
    return centre;
}
 
void init(Turtle *t)
{
    char *className = "MainWindow";
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {0};
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszClassName = className;

    if (!RegisterClass(&wc))
        die("Initialization Failed! \nClass NOT Registred.");

    HWND hwnd = CreateWindow(className, "Turtle in C", WS_OVERLAPPEDWINDOW, 250, 100, 720, 380, NULL, NULL, hInstance, NULL);

    if (!hwnd)
        die("Initialization Failed! \nWindow NOT Created");

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    t->hwnd = hwnd;

    PAINTSTRUCT ps;
    t->hdc = BeginPaint(hwnd, &ps);

    // set the center
    MoveToEx(t->hdc, 100, 150, NULL);

    MSG msg;
    while (GetMessage(&msg, hwnd, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    EndPaint(hwnd, &ps);
    UnregisterClass(className, hInstance);
}

void forward(Turtle *t, int distance)
{
    POINT curPos;
    GetCurrentPositionEx(t->hdc, &curPos);

    POINT end;
    end.x = curPos.x + distance;
    end.y = curPos.y;
    LineTo(t->hdc, end.x, end.y);
    MoveToEx(t->hdc, end.x, end.y, NULL); // change the current position
}

#endif