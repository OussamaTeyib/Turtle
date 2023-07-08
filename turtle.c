#include "turtle.h"
#include <math.h>
#include <stdio.h>

typedef struct {
    HWND hwnd;
    HDC hdc;
    double angle;
    Command *cmdQueue;
    int nCmd;
    int maxCmd;
} Turtle;

static Turtle *t = NULL;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void CreateCanvas(void);
static void ExecuteCommands(void);

void init(void)
{
    t = malloc(sizeof (Turtle));
    if (t)
    {
        t->nCmd = 0;
        t->maxCmd = MAXCMDS;
        t->cmdQueue = malloc(t->maxCmd * sizeof (Command));   
        t->angle = 0;
    }
}

static void CreateCanvas(void)
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

    HWND hwnd = CreateWindow(className, "Turtle in C", WS_OVERLAPPEDWINDOW, 250, 100, 820, 470, NULL, NULL, hInstance, NULL);

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
    static int xClient, yClient;
    static HINSTANCE hInstance;
    static char ClassName[100];
    PAINTSTRUCT ps;

    switch(message)
    {
        case WM_CREATE:
            if (t)
                t->hwnd = hwnd;

            hInstance = GetModuleHandle(NULL);
            GetClassName(hwnd, ClassName, 100);
            return 0;

        case WM_SIZE:
            xClient = LOWORD(lParam);
            yClient = HIWORD(lParam);     
            return 0;

        case WM_PAINT:
            if (t)
            {
                t->hdc = BeginPaint(t->hwnd, &ps);

                MoveToEx(t->hdc, xClient / 2, yClient / 2, NULL);
                ExecuteCommands();

                EndPaint(t->hwnd, &ps);
            }
            return 0;

        case WM_DESTROY:
            UnregisterClass(ClassName, hInstance);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void show(void)
{ 
    CreateCanvas();
    free(t->cmdQueue);
    free(t);
}

void PostCommand(cmdFunction cmd, void *params)
{
    if (!t)
       return;

    if(!t->cmdQueue)
       return;

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

static void ExecuteCommands(void)
{
    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(command.params);
    }
}
    
void __forward(void *params)
{
    ForwardParams *forwardParams = (ForwardParams *) params;

    POINT curPos;
    GetCurrentPositionEx(t->hdc, &curPos);

    double alpha = t->angle * M_PI / 180; // in radians

    POINT end;
    end.x = curPos.x + forwardParams->distance * cos(alpha);
    end.y = curPos.y - forwardParams->distance * sin(alpha); // '-' because that y-axis increases downward

    LineTo(t->hdc, end.x, end.y);
}

void __left(void *params)
{
    LeftParams *leftParams = (LeftParams *) params;
    t->angle += leftParams->angle;
}

void __goto(void *params)
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

void __circle(void *params)
{
    CircleParams *circleParams = (CircleParams *) params;

    POINT curPos;
    GetCurrentPositionEx(t->hdc, &curPos);

    RECT rect;
    rect.left = curPos.x - circleParams->r;
    rect.top = curPos.y - circleParams->r;
    rect.right = curPos.x + circleParams->r;
    rect.bottom = curPos.y + circleParams->r;

    Ellipse(t->hdc, rect.left, rect.top, rect.right, rect.bottom);
}

void __color(void *params)
{
    ColorParams *colorParams = (ColorParams *) params;

    HPEN hPen = CreatePen(PS_SOLID, 1, colorParams->color);
    SelectObject(t->hdc, hPen);

    HBRUSH hBrush = CreateSolidBrush(colorParams->color);
    SelectObject(t->hdc, hBrush);

    // delete it
}