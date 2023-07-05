#include "turtle.h"
#include <math.h>
#include <stdio.h>

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void CreateCanvas(Turtle *t);
static void ExecuteCommands(Turtle *t);

void init(Turtle *t)
{
    t->angle = 0;

    t->maxCmd = MAXCMDS;
    t->cmdQueue = malloc(t->maxCmd * sizeof (Command));
    t->nCmd = 0;
}

static void CreateCanvas(Turtle *t)
{
    char *className = "MainWindow";
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {0};
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszClassName = className;

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(className, "Turtle in C", WS_OVERLAPPEDWINDOW, 250, 100, 820, 470, NULL, NULL, hInstance, t);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, hwnd, 0, 0) > 0)
    {
        DispatchMessage(&msg);
    }
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Turtle *t;
    static int xClient, yClient;
    static HINSTANCE hInstance;
    static char ClassName[100];
    PAINTSTRUCT ps;

    switch(message)
    {
        case WM_CREATE:
            t = ((LPCREATESTRUCT) lParam)->lpCreateParams;

            t->hwnd = hwnd;
            hInstance = GetModuleHandle(NULL);
            GetClassName(hwnd, ClassName, 100);
            return 0;

        case WM_SIZE:
            xClient = LOWORD(lParam);
            yClient = HIWORD(lParam);     
            return 0;

        case WM_PAINT:
            t->hdc = BeginPaint(t->hwnd, &ps);

            MoveToEx(t->hdc, xClient / 2, yClient / 2, NULL);
            ExecuteCommands(t);

            EndPaint(t->hwnd, &ps);
            return 0;

        case WM_DESTROY:
            UnregisterClass(ClassName, hInstance);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void done(Turtle *t)
{ 
    CreateCanvas(t);
    free(t->cmdQueue);
}

void PostCommand(Turtle *t, cmdFunction cmd, void *params)
{
    if (t->nCmd == t->maxCmd)
    {
        t->maxCmd *= 2;
        Command *cmdQueue = realloc(t->cmdQueue, t->maxCmd * sizeof (Command));
        if (cmdQueue)
            t->cmdQueue = cmdQueue;
    }

    Command command = {cmd, params};
    t->cmdQueue[t->nCmd] = command;
    t->nCmd++;
}

static void ExecuteCommands(Turtle *t)
{
    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(t, command.params);
    }
}
    
void __forward(Turtle *t, void *params)
{
    ForwardParams *forwardParams = (ForwardParams *) params;

    POINT curPos;
    GetCurrentPositionEx(t->hdc, &curPos);

    double alpha = t->angle * M_PI / 180; // in radians

    POINT end;
    end.x = curPos.x + forwardParams->distance * cos(alpha);
    end.y = curPos.y - forwardParams->distance * sin(alpha); // '-' because that y-axis increases downward

    LineTo(t->hdc, end.x, end.y);
    MoveToEx(t->hdc, end.x, end.y, NULL); // change the current position
}

void __left(Turtle *t, void *params)
{
    LeftParams *leftParams = (LeftParams *) params;
    t->angle += leftParams->angle;
}

void __goto(Turtle *t, void *params)
{
    t->angle = 0;
    GotoParams *gotoParams = (GotoParams *) params;

    RECT rect;
    GetClientRect(t->hwnd, &rect);
    POINT newPos;
    newPos.x = rect.right / 2 + gotoParams->x;
    newPos.y = rect.bottom / 2 - gotoParams->y; // '-' because that y-axis increases downward
    MoveToEx(t->hdc, newPos.x, newPos.y, NULL);
}