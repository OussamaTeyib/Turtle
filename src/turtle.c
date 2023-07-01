#include "turtle.h"

void die(char *msg)
{
    MessageBox(NULL, msg, "ERROR", MB_ICONEXCLAMATION | MB_OK);    
    exit(EXIT_FAILURE);
}
 
void init(Turtle *t)
{
    t->cmdQueue = malloc(sizeof (Command) * 50); // max is 50
    t->nCmd = 0;
}

void done(Turtle *t)
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
    t->hdc = GetDC(hwnd);

    MoveToEx(t->hdc, 100, 150, NULL);

    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(t, command.params);
    }

    MSG msg;
    while (GetMessage(&msg, t->hwnd, 0, 0) > 0)
        DispatchMessage(&msg);
  
    ReleaseDC(t->hwnd, t->hdc);
    free(t->cmdQueue);
    UnregisterClass(className, hInstance);   
}

void PostCommand(Turtle *t, cmdFunction cmd, void *params)
{
    Command command = {cmd, params};
    t->cmdQueue[t->nCmd] = command;
    t->nCmd++;
}

POINT GetCentre(HWND hwnd)
{
    POINT centre;
    RECT rect;

    GetClientRect(hwnd, &rect);
    centre.x = rect.right / 2;
    centre.y = rect.bottom / 2;
    return centre;
}
    
void ExecuteForward(Turtle *t, void *params)
{
    ForwardParams *forwardParams = (ForwardParams *) params;

    POINT curPos;
    GetCurrentPositionEx(t->hdc, &curPos);

    POINT end;
    end.x = curPos.x + forwardParams->distance;
    end.y = curPos.y;
    LineTo(t->hdc, end.x, end.y);
    MoveToEx(t->hdc, end.x, end.y, NULL); // change the current position
}