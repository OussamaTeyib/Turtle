// to use M_PI
#define _USE_MATH_DEFINES

#include <turtle.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>

#define MAX_CMDS 100

typedef void (*cmdFunction) (void *);

typedef struct {
    cmdFunction cmd;
    void *params;
} Command;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void CreateCanvas(void);
static void cleanup(void);
static void PostCommand(cmdFunction cmd, void *params);
static void LinesToPolygon(void);
static void ExecuteCommands(void);
static void __move(void *params);
static void __polygon(void *params);
static void __circle(void *params);
static COLORREF GetColor(const char *szColor);

enum color {
    WHITE = RGB(255, 255, 255),
    BLACK = RGB(0, 0, 0),
    RED = RGB(255, 0, 0),
    GREEN = RGB(0, 255, 0),
    BLUE = RGB(0, 0, 255),
    YELLOW = RGB(255, 255, 0),
};

typedef struct {
    HWND hwnd;
    HDC hdc;
    POINT pos;
    double angle;
    COLORREF pencolor;
    COLORREF fillcolor;
    bool pendown;
    bool fill;
    Command *cmdQueue;
    int nCmd;
    int maxCmd;
} Turtle;  

typedef struct
{ 
    POINT dest;
    COLORREF pencolor;
    COLORREF fillcolor;
    bool pendown;
    bool fill;
} MoveParams;

typedef struct
{ 
    POINT *apt;
    int count;
    COLORREF fillcolor;
    bool fill;
} PolygonParams;

typedef struct {
    int r;
    COLORREF pencolor;
    COLORREF fillcolor;
    bool pendown;
    bool fill;
} CircleParams;

static Turtle *t = NULL;

void init(void)
{
    // Already initialized?
    if (t)
        cleanup();

    t = malloc(sizeof (Turtle));
    if (!t)
        return;

    t->hwnd = NULL;
    t->hdc = NULL;
    t->pos = (POINT) {0, 0};
    t->angle = 0.0;
    t->pencolor = BLACK;
    t->fillcolor = BLACK;
    t->pendown = true;
    t->fill = false;

    t->maxCmd = MAX_CMDS;
    t->cmdQueue = calloc(t->maxCmd, sizeof (Command));
    if (!t->cmdQueue)
        return;

    t->nCmd = 0;
    MoveParams *initMove = malloc(sizeof (MoveParams));
    initMove->dest.x = 0, initMove->dest.y = 0;
    initMove->pendown = false;
    PostCommand(__move, initMove);
}

static void CreateCanvas(void)
{
    const char *className = "MainWindow";
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {0};
    // to re-paint the client area when the window's size changes
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszClassName = className;

    if (!RegisterClass(&wc))
        return;

    int xScreen = GetSystemMetrics(SM_CXSCREEN);
    int yScreen = GetSystemMetrics(SM_CYSCREEN);

    RECT window;
    window.left = xScreen / 4;
    window.right = xScreen / 2;
    window.top = yScreen / 8;
    window.bottom = 6 * yScreen / 8;

    HWND hwnd = CreateWindow(className, "C Turtle Graphics", WS_OVERLAPPEDWINDOW, window.left, window.top, window.right, window.bottom, NULL, NULL, hInstance, NULL);

    if (!hwnd)
        return;

    t->hwnd = hwnd;

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, hwnd, 0, 0) > 0)
        DispatchMessage(&msg);

    UnregisterClass(className, hInstance);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int xClient, yClient;
    PAINTSTRUCT ps;

    switch(message)
    {
        case WM_SIZE:
            xClient = LOWORD(lParam);
            yClient = HIWORD(lParam);  
            return 0;

        case WM_PAINT:
            t->hdc = BeginPaint(t->hwnd, &ps);

            SetMapMode(t->hdc, MM_ISOTROPIC);
            SetWindowExtEx(t->hdc, 1, 1, NULL);
            SetViewportExtEx(t->hdc, 1, -1, NULL); // '-' to have the y-axis increase upward
            SetViewportOrgEx(t->hdc, xClient / 2, yClient / 2, NULL);
 
            ExecuteCommands();

            EndPaint(t->hwnd, &ps);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void show(void)
{ 
    if (!t || !t->cmdQueue)
       return;

    LinesToPolygon();
    CreateCanvas();
    cleanup();
}

static void cleanup(void)
{
    for (int i = 0; i < t->nCmd; i++)
    {
        if (t->cmdQueue[i].cmd == __polygon)
        {
            PolygonParams *temp = (PolygonParams *) t->cmdQueue[i].params;
            free(temp->apt);
        }
        free(t->cmdQueue[i].params);
    }

    free(t->cmdQueue);
    free(t);
    // prevent any re-use before re-initialization
    t = NULL;
}

static void PostCommand(cmdFunction cmd, void *params)
{
    if (!t || !t->cmdQueue)
       return;

    if (t->nCmd == t->maxCmd)
    {
        t->maxCmd *= 2;
        Command *cmdQueue = realloc(t->cmdQueue, t->maxCmd * sizeof (Command));
        if (!cmdQueue)
            return;

        t->cmdQueue = cmdQueue;
    }
    
    t->cmdQueue[t->nCmd++] = (Command) {cmd, params};
}

static void LinesToPolygon(void)
{
    // skip if there is only the initial move in the cmdQueue
    if (1 == t->nCmd)
       return;

    Command *cmdQueue = calloc(t->maxCmd, sizeof (Command)); // to be dynamically changed
    if (!cmdQueue)
       return;

    int count = 0;

    for (int i = 0; i < t->nCmd; i++)
    {
        cmdQueue[count++] = t->cmdQueue[i];
        // check if this position is repeaated
        if (t->cmdQueue[i].cmd == __move)
        {
            MoveParams *curPt = (MoveParams *) t->cmdQueue[i].params;
            bool found = false;

            int index;
            for (int j = 0; j < i; j++)
            {
                if (t->cmdQueue[j].cmd != __move)
                    continue;

                MoveParams *pt = (MoveParams *) t->cmdQueue[j].params;
                if (curPt->dest.x == pt->dest.x && curPt->dest.y == pt->dest.y)
                {
                    // Get the closest occurence
                    index = j;
                    found = true;
                }
            }

            if (found)
            {
                PolygonParams *polygonParams = malloc(sizeof (PolygonParams));
                if (!polygonParams)
                    return;

                polygonParams->count = 0;
                for (int k = index; k <= i; k++)
                {
                    if (t->cmdQueue[k].cmd != __move)
                        continue;

                    polygonParams->count++;
                } 

                polygonParams->apt = calloc(polygonParams->count, sizeof (POINT));
                if (!polygonParams->apt)
                    return;

                for (int k = index, aptCount = 0; k <= i; k++)
                {
                    if (t->cmdQueue[k].cmd != __move)
                        continue;

                    MoveParams *temp = (MoveParams *) t->cmdQueue[k].params;

                    polygonParams->apt[aptCount++] = temp->dest;
                    // Get filling info from the last line
                    polygonParams->fill = temp->fill;
                    polygonParams->fillcolor = temp->fillcolor;
                }

                cmdQueue[count++] = (Command) {__polygon, polygonParams};
            }
        }
    }

    free(t->cmdQueue);
    t->cmdQueue = cmdQueue;
    t->nCmd = count;
}

static void ExecuteCommands(void)
{
    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(command.params);
    }
}

static void __move(void *params)
{
    MoveParams *moveParams = (MoveParams *) params;

    HPEN hPen, hPrevPen;
    if (moveParams->pendown)
        hPen = CreatePen(PS_SOLID, 1, moveParams->pencolor);
    else
        hPen = GetStockObject(NULL_PEN);   

    hPrevPen = SelectObject(t->hdc, hPen);

    LineTo(t->hdc, moveParams->dest.x, moveParams->dest.y);

    // deselecting hPen before deleting it
    SelectObject(t->hdc, hPrevPen);

    if (moveParams->pendown)
        DeleteObject(hPen);
}

static void __polygon(void *params)
{
    PolygonParams *polygonParams = (PolygonParams *) params;

    HBRUSH hBrush, hPrevBrush;
    if (polygonParams->fill)
        hBrush = CreateSolidBrush(polygonParams->fillcolor);
    else
        hBrush = GetStockObject(NULL_BRUSH);   

    hPrevBrush = SelectObject(t->hdc, hBrush);
    SelectObject(t->hdc, GetStockObject(NULL_PEN));
    SetPolyFillMode(t->hdc, WINDING);

    Polygon(t->hdc, polygonParams->apt, polygonParams->count);

    // deselect hBrush before deleting it
    SelectObject(t->hdc, hPrevBrush);

    if (polygonParams->fill)
        DeleteObject(hBrush);
}

static void __circle(void *params)
{
    CircleParams *circleParams = (CircleParams *) params;

    POINT curPos;
    GetCurrentPositionEx(t->hdc, &curPos);

    RECT rect;
    rect.left = curPos.x - circleParams->r;
    rect.top = curPos.y + 2 * circleParams->r;
    rect.right = curPos.x + circleParams->r;
    rect.bottom = curPos.y;

    HPEN hPen, hPrevPen;
    if (circleParams->pendown)
        hPen = CreatePen(PS_SOLID, 1, circleParams->pencolor);
    else
        hPen = GetStockObject(NULL_PEN);

    HBRUSH hBrush, hPrevBrush;
    if (circleParams->fill)
        hBrush = CreateSolidBrush(circleParams->fillcolor);
    else
        hBrush = GetStockObject(NULL_BRUSH);

    hPrevPen = SelectObject(t->hdc, hPen);
    hPrevBrush = SelectObject(t->hdc, hBrush);

    Ellipse(t->hdc, rect.left, rect.top, rect.right, rect.bottom);

    SelectObject(t->hdc, hPrevPen);
    if (circleParams->pendown)
        DeleteObject(hPen);

    SelectObject(t->hdc, hPrevBrush);
    if (circleParams->fill)
        DeleteObject(hBrush);
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
    moveParams->pendown = t->pendown;
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
    moveParams->pendown = t->pendown;
    moveParams->fill = t->fill;
    
    PostCommand(__move, moveParams);

    t->pos.x = moveParams->dest.x;
    t->pos.y = moveParams->dest.y;
}

void home(void)
{
    if (!t || !t->cmdQueue)
       return;

    setpos(0, 0);
    setheading(0.0);
}

static COLORREF GetColor(const char *szColor)
{
    // handle all chars cases
 
    if (!strncmp(szColor, "white", 5))
        return WHITE;
    else if (!strncmp(szColor, "black", 5))
        return BLACK;
    else if (!strncmp(szColor, "red", 3))
        return RED;
    else if (!strncmp(szColor, "green", 5))
        return GREEN;
    else if (!strncmp(szColor, "blue", 4))
        return BLUE;
    else if (!strncmp(szColor, "yellow", 6))
        return YELLOW;
    else
        return -1;
}

void color(const char *szColor)
{
    if (!t || !t->cmdQueue || !szColor)
       return;

    pencolor(szColor);
    fillcolor(szColor); 
}

void pencolor(const char *szColor)
{
    if (!t || !t->cmdQueue || !szColor)
       return;

    COLORREF color;
    if ((color = GetColor(szColor)) == (COLORREF) -1)
        return;

    t->pencolor = color; 
}

void fillcolor(const char *szColor)
{
    if (!t || !t->cmdQueue || !szColor)
       return;

    COLORREF color;
    if ((color = GetColor(szColor)) == (COLORREF) -1)
        return;

    t->fillcolor = color; 
}

void penup(void)
{
    if (!t || !t->cmdQueue)
        return;

     t->pendown = false;
}

void pendown(void)
{
    if (!t || !t->cmdQueue)
        return;

     t->pendown= true;
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

void circle(int r)
{
    if (!t || !t->cmdQueue)
       return;

    CircleParams *circleParams = malloc(sizeof (CircleParams));
    circleParams->r = r;
    circleParams->pencolor = t->pencolor;
    circleParams->fillcolor = t->fillcolor;
    circleParams->pendown = t->pendown;
    circleParams->fill = t->fill;

    PostCommand(__circle, circleParams);
}