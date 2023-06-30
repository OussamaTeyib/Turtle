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
} Turtle;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CHAR:
            if (wParam == 'q' || 'Q' == wParam)
                DestroyWindow(hWnd);
      
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;       
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);  
}
 
int init(Turtle t)
{
    char *className = "MainWindow";
    WNDCLASS wc = {0};
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszClassName = className;

    if (!RegisterClass(&wc))
        die("Initialization Failed! \nClass NOT Registred.");

    HWND hwnd = CreateWindow(className, "Turtle in C", WS_OVERLAPPEDWINDOW, 250, 100, 720, 380, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwnd)
        die("Initialization Failed! \nWindow NOT Created");

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    t.hwnd = hwnd;

    MSG msg;
    while (GetMessage(&msg, hwnd, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    UnregisterClass(className, GetModuleHandle(NULL));
    return msg.wParam;
}

#endif