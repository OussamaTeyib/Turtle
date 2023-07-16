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
static void LinesToPolygon(void);
static void ExecuteCommands(void);
static void __move(void *params);
static void __polygon(void *params);
static void __circle(void *params);
static COLORREF GetColor(const char *szColor);

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
        t->pendown = true;
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

    int xScreen = GetSystemMetrics(SM_CXSCREEN);
    int yScreen = GetSystemMetrics(SM_CYSCREEN);

    RECT window;
    window.left = xScreen / 4;
    window.right = xScreen / 2;
    window.top = yScreen / 8;
    window.bottom = 6 * yScreen / 8;

    HWND hwnd = CreateWindow(className, "C Turtle Graphics", WS_OVERLAPPEDWINDOW, window.left, window.top, window.right, window.bottom, NULL, NULL, hInstance, NULL);

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
            
            LinesToPolygon();

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
    {
        if (t->cmdQueue[i].cmd == __polygon)
        {
            PolygonParams *polygonParams = (PolygonParams *) t->cmdQueue[i].params;
            free(polygonParams->apt);
        }
        free(t->cmdQueue[i].params);
    }

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

static void LinesToPolygon(void)
{
    Command *cmdQueue = malloc(t->maxCmd * sizeof (Command)); // to be changed
    int count = 0;

    for (int i = 0; i < t->nCmd; )
    {
        if (t->cmdQueue[i].cmd == __move)
        {
            MoveParams *curPt = (MoveParams *) t->cmdQueue[i].params;

            bool found = false;

            for (int j = i + 1; j < t->nCmd; j++)
            {
                if (t->cmdQueue[j].cmd != __move)
                    continue;

                MoveParams *pt = (MoveParams *) t->cmdQueue[j].params;
                if (curPt->dest.x == pt->dest.x && curPt->dest.y == pt->dest.y)
                {
                    PolygonParams *polygonParams = malloc(sizeof (PolygonParams));
                    polygonParams->count = j - i + 1;
                    polygonParams->apt = malloc(polygonParams->count * sizeof (POINT));

                    for (int k = i; k <= j; k++)
                    {
                        if (t->cmdQueue[k].cmd != __move)
                            continue;

                        MoveParams *temp = (MoveParams *) t->cmdQueue[k].params;

                        polygonParams->apt[k - i] = temp->dest;
                        polygonParams->fill = temp->fill;
                        polygonParams->fillcolor = temp->fillcolor;
                    }

                    found = true;
                      
                    for (int k = i; k <= j; k++)
                    {
                        cmdQueue[count++] = t->cmdQueue[k];
                    }

                    cmdQueue[count].cmd = __polygon;
                    cmdQueue[count].params = polygonParams;
                    count++;
                     
                    i = j;
                    break;
                }
            }

            if (!found)
            {
                // check if repaeted
                cmdQueue[count++] = t->cmdQueue[i];
                i++;
            }             
        }
        else
        {
            cmdQueue[count++] = t->cmdQueue[i];
            i++;
        }
    
    }

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

    HPEN hPen;
    if (moveParams->pendown)
        hPen = CreatePen(PS_SOLID, 1, moveParams->pencolor);
    else
        hPen = GetStockObject(NULL_PEN);   

    SelectObject(t->hdc, hPen);

    POINT dest;
    RECT rect;
    GetClientRect(t->hwnd, &rect);
    dest.x = rect.right / 2 + moveParams->dest.x;
    dest.y = rect.bottom / 2 - moveParams->dest.y; // '-' because that y-axis increases downward

    LineTo(t->hdc, dest.x, dest.y);

    if (moveParams->pendown)
        DeleteObject(hPen);
}

static void __polygon(void *params)
{
    PolygonParams *polygonParams = (PolygonParams *) params;

    HBRUSH hBrush;
    if (polygonParams->fill)
        hBrush = CreateSolidBrush(polygonParams->fillcolor);
    else
        hBrush = GetStockObject(NULL_BRUSH);   

    SelectObject(t->hdc, hBrush);
    SelectObject(t->hdc, GetStockObject(NULL_PEN));
    SetPolyFillMode(t->hdc, WINDING);

    RECT rect;
    GetClientRect(t->hwnd, &rect);
   
    POINT apt[polygonParams->count];
    for (int i = 0; i < polygonParams->count; i++)
    {
        apt[i].x = rect.right / 2 + polygonParams->apt[i].x;
        apt[i].y = rect.bottom / 2 - polygonParams->apt[i].y; 
    }

    Polygon(t->hdc, apt, polygonParams->count);

    if (polygonParams->fill)
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
    if (!t || !t->cmdQueue || !szColor)
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
    if (!t || !t->cmdQueue || !szColor)
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
    if (!t || !t->cmdQueue || !szColor)
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
    if (circleParams->pendown)
        hPen = CreatePen(PS_SOLID, 1, circleParams->pencolor);
    else
        hPen = GetStockObject(NULL_PEN);
        

    HBRUSH hBrush;
    if (circleParams->fill)
        hBrush = CreateSolidBrush(circleParams->fillcolor);
    else
        hBrush = GetStockObject(NULL_BRUSH);

    SelectObject(t->hdc, hPen);
    SelectObject(t->hdc, hBrush);

    Ellipse(t->hdc, rect.left, rect.top, rect.right, rect.bottom);

    if (circleParams->pendown)
        DeleteObject(hPen);
    if (circleParams->fill)
        DeleteObject(hBrush);
}