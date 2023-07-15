#include <turtle.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>

#define MAX_CMDS 50

typedef void (*cmdFunction) (void *);

typedef struct {
    cmdFunction cmd;
    void *params;
} Command;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void CreateCanvas(void);
static void cleanup(void);
static void PostCommand(cmdFunction cmd, void *params);
static void ExecuteCommands(void);
static void __move(void *params);
static void __circle(void *params);
static COLORREF GetColor(const char *szColor);

typedef struct {
    HWND hwnd;
    HDC hdc;
    POINT pos;
    double angle;
    COLORREF pencolor;
    COLORREF fillcolor;
    bool isPenUp;
    bool fill;
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
        t->maxCmd = MAX_CMDS;
        t->cmdQueue = malloc(t->maxCmd * sizeof (Command));
        t->nCmd = 0;

        t->pos.x = 0, t->pos.y = 0;
        t->angle = 0.0;
        t->pencolor = RGB(0, 0, 0);
        t->fillcolor = RGB(0, 0, 0);
        t->isPenUp = false;
        t->fill = false;

        // add 'move to (0, 0)' to the cmdQueue
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
            
            // process commands

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
    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(command.params);
    }
}

typedef struct
{ 
    POINT dest;
    COLORREF pencolor;
    COLORREF fillcolor;
    bool isPenUp;
    bool fill;
} MoveParams;

static void __move(void *params)
{
    MoveParams *moveParams = (MoveParams *) params;

    HPEN hPen;
    if (moveParams->isPenUp)
        hPen = GetStockObject(NULL_PEN);
    else
        hPen = CreatePen(PS_SOLID, 1, moveParams->pencolor);

    SelectObject(t->hdc, hPen);

    POINT dest;
    RECT rect;
    GetClientRect(t->hwnd, &rect);
    dest.x = rect.right / 2 + moveParams->dest.x;
    dest.y = rect.bottom / 2 - moveParams->dest.y; // '-' because that y-axis increases downward

    LineTo(t->hdc, dest.x, dest.y);

    if (!moveParams->isPenUp)
        DeleteObject(hPen);
}

void forward(int distance)
{
    if (!t || !t->cmdQueue)
       return;

    MoveParams *moveParams = malloc(sizeof (MoveParams));

    double alpha = t->angle * M_PI / 180.0; // in radians

    moveParams->dest.x = round(t->pos.x + distance * cos(alpha));
    moveParams->dest.y = round(t->pos.y + distance * sin(alpha));

    moveParams->pencolor = t->pencolor;
    moveParams->fillcolor = t->fillcolor;
    moveParams->isPenUp = t->isPenUp;
    moveParams->fill = t->fill;
    
    PostCommand(__move, moveParams);

    t->pos.x = moveParams->dest.x;
    t->pos.y = moveParams->dest.y;
}

void left(double angle)
{
    if (!t || !t->cmdQueue)
       return;

    t->angle += angle;
}

void right(double angle)
{   
    if (!t || !t->cmdQueue)
       return;

    t->angle -= angle;
}

void setheading(double angle)
{
    if (!t || !t->cmdQueue)
       return;

    t->angle = angle;
}

void setpos(int x, int y)
{
    if (!t || !t->cmdQueue)
       return;

    MoveParams *moveParams = malloc(sizeof (MoveParams));

    moveParams->dest.x = x;
    moveParams->dest.y = y;

    moveParams->pencolor = t->pencolor;
    moveParams->fillcolor = t->fillcolor;
    moveParams->isPenUp = t->isPenUp;
    moveParams->fill = t->fill;
    
    PostCommand(__move, moveParams);

    t->pos.x = moveParams->dest.x;
    t->pos.y = moveParams->dest.y;
}


static COLORREF GetColor(const char *szColor)
{
    // handle all chars cases
 
    if (!strncmp(szColor, "white", 5))
        return RGB(255, 255, 255);
    else if (!strncmp(szColor, "black", 5))
        return RGB(0, 0, 0);
    else if (!strncmp(szColor, "red", 3))
        return RGB(255, 0, 0);
    else if (!strncmp(szColor, "green", 5))
        return RGB(0, 255, 0);
    else if (!strncmp(szColor, "blue", 4))
        return RGB(0, 0, 255);
    else if (!strncmp(szColor, "yellow", 6))
        return RGB(255, 255, 0);
    else
        return -1;
}

void color(const char *szColor)
{
    if (!t || !t->cmdQueue)
       return;

    if (!szColor)
       return;

    COLORREF color;
    if ((color = GetColor(szColor)) != (COLORREF) -1)
    {
        t->pencolor = color;
        t->fillcolor = color;
    }
    else
        return;
}

void pencolor(const char *szColor)
{
    if (!t || !t->cmdQueue)
       return;

    if (!szColor)
       return;

    COLORREF color;
    if ((color = GetColor(szColor)) != (COLORREF) -1)
    {
        t->pencolor = color;
    }
    else
        return;
}

void fillcolor(const char *szColor)
{
    if (!t || !t->cmdQueue)
       return;

    if (!szColor)
       return;

    COLORREF color;
    if ((color = GetColor(szColor)) != (COLORREF) -1)
    {
        t->fillcolor = color;
    }
    else
        return;
}

void penup(void)
{
    if (!t || !t->cmdQueue)
        return;

     t->isPenUp = true;
}

void pendown(void)
{
    if (!t || !t->cmdQueue)
        return;

     t->isPenUp = false;
}

void begin_fill(void)
{
    if (!t || !t->cmdQueue)
        return;

     t->fill = true;
}

void end_fill(void)
{
    if (!t || !t->cmdQueue)
        return;

     t->fill = false;
}

typedef struct {
    int r;
    COLORREF pencolor;
    COLORREF fillcolor;
    bool isPenUp;
    bool fill;
} CircleParams;

void circle(int r)
{
    if (!t || !t->cmdQueue)
       return;

    CircleParams *circleParams = malloc(sizeof (CircleParams));
    circleParams->r = r;
    circleParams->pencolor = t->pencolor;
    circleParams->fillcolor = t->fillcolor;
    circleParams->isPenUp = t->isPenUp;
    circleParams->fill = t->fill;

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

    HPEN hPen;
    if (circleParams->isPenUp)
        hPen = GetStockObject(NULL_PEN);
    else
        hPen = CreatePen(PS_SOLID, 1, circleParams->pencolor);

    HBRUSH hBrush;
    if (circleParams->fill)
        hBrush = CreateSolidBrush(circleParams->fillcolor);
    else
        hBrush = GetStockObject(NULL_BRUSH);

    SelectObject(t->hdc, hPen);
    SelectObject(t->hdc, hBrush);

    Ellipse(t->hdc, rect.left, rect.top, rect.right, rect.bottom);

    if (!circleParams->isPenUp)
        DeleteObject(hPen);
    if (circleParams->fill)
        DeleteObject(hBrush);
}