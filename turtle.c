#include "turtle.h"
#include <windows.h>
#include <math.h>

#define MAX_CMDS 50

typedef void (*cmdFunction) (void *);

typedef struct {
    cmdFunction cmd;
    void *params;
} Command;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void CreateCanvas(void);
static void cleanup(void);
static void ExecuteCommands(void);
static void PostCommand(cmdFunction cmd, void *params);
static void __forward(void *params);
static void __left(void *params);
static void __setpos(void *params);
static void __circle(void *params);
static void __color(void *params);

typedef struct {
    HWND hwnd;
    HDC hdc;
    double angle;
    Command *cmdQueue;
    int nCmd;
    int maxCmd;
} Turtle;

static Turtle *t = NULL;

void init(void)
{
    t = malloc(sizeof (Turtle));
    if (t)
    {
        t->nCmd = 0;
        t->maxCmd = MAX_CMDS;
        t->cmdQueue = malloc(t->maxCmd * sizeof (Command));
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
            t->hwnd = hwnd;

            hInstance = GetModuleHandle(NULL);
            GetClassName(t->hwnd, ClassName, 100);
            return 0;

        case WM_SIZE:
            xClient = LOWORD(lParam);
            yClient = HIWORD(lParam);     
            return 0;

        case WM_PAINT:
            t->hdc = BeginPaint(t->hwnd, &ps);

            MoveToEx(t->hdc, xClient / 2, yClient / 2, NULL);
            ExecuteCommands();

            EndPaint(t->hwnd, &ps);
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
    if (!t || !t->cmdQueue)
       return;

    CreateCanvas();
    cleanup();
}

static void cleanup(void)
{
    for (int i = 0; i < t->nCmd; i++)
        free(t->cmdQueue[i].params);

    free(t->cmdQueue);
    free(t);
}

static void PostCommand(cmdFunction cmd, void *params)
{
    if (!t || !t->cmdQueue)
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
    t->angle = 0.0;
    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(command.params);
    }
}
 
typedef struct
{
    int distance;
} ForwardParams;

void forward(int distance)
{
    ForwardParams *forwardParams = malloc(sizeof (ForwardParams));
    forwardParams->distance = distance;
    PostCommand(__forward, forwardParams);
}
  
static void __forward(void *params)
{
    ForwardParams *forwardParams = (ForwardParams *) params;

    POINT curPos;
    GetCurrentPositionEx(t->hdc, &curPos);

    double alpha = t->angle * M_PI / 180.0; // in radians

    POINT end;
    end.x = curPos.x + forwardParams->distance * cos(alpha);
    end.y = curPos.y - forwardParams->distance * sin(alpha); // '-' because that y-axis increases downward

    LineTo(t->hdc, end.x, end.y);
}

typedef struct 
{
    double angle;
} LeftParams;

void left(double angle)
{
    LeftParams *leftParams = malloc(sizeof (LeftParams));
    leftParams->angle = angle;
    PostCommand(__left, leftParams);
}

void right(double angle)
{   
    LeftParams *leftParams = malloc(sizeof (LeftParams));
    leftParams->angle = -angle;
    PostCommand(__left, leftParams);
}

static void __left(void *params)
{
    LeftParams *leftParams = (LeftParams *) params;
    t->angle += leftParams->angle;
}

typedef struct
{
    int x, y;
} SetposParams;

void setpos(int x, int y)
{
    SetposParams *setposParams = malloc(sizeof (SetposParams));
    setposParams->x = x, setposParams->y = y;
    PostCommand(__setpos, setposParams);
}

static void __setpos(void *params)
{
    SetposParams *setposParams = (SetposParams *) params;

    RECT rect;
    GetClientRect(t->hwnd, &rect);
    POINT newPos;
    newPos.x = rect.right / 2 + setposParams->x;
    newPos.y = rect.bottom / 2 - setposParams->y; // '-' because that y-axis increases downward
    MoveToEx(t->hdc, newPos.x, newPos.y, NULL);
}

typedef struct
{
    int r;
} CircleParams;

void circle(int r)
{
    CircleParams *circleParams = malloc(sizeof (CircleParams));
    circleParams->r = r;
    PostCommand(__circle, circleParams);
}

static void __circle(void *params)
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

typedef struct
{
    int color;
} ColorParams;

void color(int color)
{
     ColorParams *colorParams = malloc(sizeof (ColorParams));
     colorParams->color = color;
     PostCommand(__color, colorParams);
}

static void __color(void *params)
{
    ColorParams *colorParams = (ColorParams *) params;

    HPEN hPen = CreatePen(PS_SOLID, 1, colorParams->color);
    SelectObject(t->hdc, hPen);

    HBRUSH hBrush = CreateSolidBrush(colorParams->color);
    SelectObject(t->hdc, hBrush);

    // delete it
}