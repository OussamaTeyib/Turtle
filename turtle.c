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

typedef struct {
    cmdFunction cmd;
    void *params;
} CommandEx;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void CreateCanvas(void);
static void cleanup(void);
static void PostCommand(cmdFunction cmd, void *params, bool isEx);
static void HandleExCommands(void);
static void ExecuteCommands(void);
static void __polygon(void *params);
static void __move(void *params);
static void __forward(void *params);
static void __left(void *params);
static void __setheading(void *params);
static void __setpos(void *params);
static void __circle(void *params);
static void __color(void *params);

typedef struct {
    HWND hwnd;
    HDC hdc;
    double angle;
    COLORREF color;
    Command *cmdQueue;
    int nCmd;
    int maxCmd;
    CommandEx *cmdQueueEx;
    int nCmdEx;
    int maxCmdEx;
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

        t->maxCmdEx = MAX_CMDS;
        t->cmdQueueEx = malloc(t->maxCmdEx * sizeof (CommandEx));
        t->nCmdEx = 1; // 0 is reserved
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
            t->color = RGB(0, 0, 0);
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
 
    for (int i = 0; i < t->nCmdEx; i++)
        free(t->cmdQueueEx[i].params);

    free(t->cmdQueue);
    free(t->cmdQueueEx);
    free(t);
}

static void PostCommand(cmdFunction cmd, void *params, bool isEx)
{
    if (!t || !t->cmdQueue || !t->cmdQueueEx)
       return;

    if (!isEx)
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
    else
    {
        if (t->nCmdEx == t->maxCmdEx)
        {
            t->maxCmdEx *= 2;
            CommandEx *cmdQueue = realloc(t->cmdQueueEx, t->maxCmdEx * sizeof (CommandEx));
            if (cmdQueue)
                t->cmdQueueEx = cmdQueue;
        }
    
        CommandEx command = {cmd, params};
        t->cmdQueueEx[t->nCmdEx] = command;
        t->nCmdEx++;
    }
}

typedef struct
{ 
    POINT dest;
    COLORREF color;
    bool isPenUp;
} MoveParams;

static void ExecuteCommands(void)
{
    t->angle = 0.0;
    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(command.params);
    }

    RECT rect;
    GetClientRect(t->hwnd, &rect);
    MoveParams *moveParams = malloc(sizeof (MoveParams));
    moveParams->dest.x = rect.right / 2, moveParams->dest.y = rect.bottom / 2;
    moveParams->isPenUp = true;

    t->cmdQueueEx[0].cmd = __move;
    t->cmdQueueEx[0].params = moveParams;

    HandleExCommands();

    for (int i = 0; i < t->nCmdEx; i++)
    {
         CommandEx command = t->cmdQueueEx[i];
         command.cmd(command.params);
    }
}

typedef struct {
   POINT *apt;
   int count;
   COLORREF color;
   bool fill;
} PolygonParams;

static void HandleExCommands(void)
{
    CommandEx *cmdQueue = malloc(MAX_CMDS * sizeof (CommandEx));
    int cmdQueueCount = 0;

    for (int i = 0; i < t->nCmdEx;)
    {
        if (t->cmdQueueEx[i].cmd == __move)
        {
            bool repeated = false;
            MoveParams *moveParams = (MoveParams *) t->cmdQueueEx[i].params;

            for (int j = i + 1; j < t->nCmdEx; j++)
            {   
                if (t->cmdQueueEx[j].cmd != __move)
                    break;

                MoveParams *temp = (MoveParams *) t->cmdQueueEx[j].params;

                if (temp->dest.x == moveParams->dest.x && temp->dest.y == moveParams->dest.y)
                {
                    repeated = true;
                    /* for (int k = i; k <= j; k++)
                    {
                        cmdQueue[cmdQueueCount]= t->cmdQueueEx[k];
                        cmdQueueCount++;
                    } */

                    PolygonParams *polygonParams = malloc(sizeof (PolygonParams));
                    polygonParams->count = j - i + 1;
                    polygonParams->apt = malloc(polygonParams->count * sizeof (POINT));

                    //   ---   free apt !!----

                    polygonParams->fill = true;

                    for (int k = i; k <= j; k++)
                    {
                        MoveParams *temp = (MoveParams *) t->cmdQueueEx[k].params;
                        polygonParams->apt[k - i] = temp->dest;
                        polygonParams->color = temp->color; // the color of is the clr of the last line
                    }

                    cmdQueue[cmdQueueCount].cmd = __polygon;
                    cmdQueue[cmdQueueCount].params = polygonParams;
                    cmdQueueCount++;
                    
                    i = j;
                    break;                    
                }
            }

            if (!repeated)
            {
                cmdQueue[cmdQueueCount] = t->cmdQueueEx[i];
                cmdQueueCount++;
                i++;
            }    
        }
        else
        {
            cmdQueue[cmdQueueCount] = t->cmdQueueEx[i];
            cmdQueueCount++;
            i++;
        }
    }

    t->cmdQueueEx = cmdQueue;
    t->nCmdEx = cmdQueueCount;
}

static void __polygon(void *params)
{
    PolygonParams *polygonParams = (PolygonParams *) params;

    if (polygonParams->fill)
    {
       HPEN hPen = CreatePen(PS_SOLID, 1, polygonParams->color);
       SelectObject(t->hdc, hPen);

       HBRUSH hBrush = CreateSolidBrush(polygonParams->color);
       SelectObject(t->hdc, hBrush);

       // clean - up
    }

    Polygon(t->hdc, polygonParams->apt, polygonParams->count);
}

static void __move(void *params)
{
    MoveParams *moveParams = (MoveParams *) params;

    if (moveParams->isPenUp)
        SelectObject(t->hdc, GetStockObject(NULL_PEN));
    else
    {
        HPEN hPen = CreatePen(PS_SOLID, 1, moveParams->color);
        SelectObject(t->hdc, hPen);

        // delete it
    }

    LineTo(t->hdc, moveParams->dest.x, moveParams->dest.y);
}
 
typedef struct
{
    int distance;
} ForwardParams;

void forward(int distance)
{
    ForwardParams *forwardParams = malloc(sizeof (ForwardParams));
    forwardParams->distance = distance;
    PostCommand(__forward, forwardParams, false);
}
  
static void __forward(void *params)
{
    ForwardParams *forwardParams = (ForwardParams *) params;

    POINT curPos;
    GetCurrentPositionEx(t->hdc, &curPos);

    double alpha = t->angle * M_PI / 180.0; // in radians

    POINT end;
    end.x = round(curPos.x + forwardParams->distance * cos(alpha));
    end.y = round(curPos.y - forwardParams->distance * sin(alpha)); // '-' because that y-axis increases downward

    // LineTo(t->hdc, end.x, end.y);
    MoveToEx(t->hdc, end.x, end.y, NULL);

    MoveParams *moveParams = malloc(sizeof (MoveParams));
    moveParams->dest.x = end.x, moveParams->dest.y = end.y;
    moveParams->color = t->color;
    moveParams->isPenUp = false;
    PostCommand(__move, moveParams, true);
}

typedef struct 
{
    double angle;
} LeftParams;

void left(double angle)
{
    LeftParams *leftParams = malloc(sizeof (LeftParams));
    leftParams->angle = angle;
    PostCommand(__left, leftParams, false);
}

void right(double angle)
{   
    LeftParams *leftParams = malloc(sizeof (LeftParams));
    leftParams->angle = -angle;
    PostCommand(__left, leftParams, false);
}

static void __left(void *params)
{
    LeftParams *leftParams = (LeftParams *) params;
    t->angle += leftParams->angle;
}

void setheading(double angle)
{
    LeftParams *leftParams = malloc(sizeof (LeftParams));
    leftParams->angle = angle;
    PostCommand(__setheading, leftParams, false);
}

static void __setheading(void *params)
{
    LeftParams *leftParams = (LeftParams *) params;
    t->angle = leftParams->angle;
}

typedef struct
{
    int x, y;
} SetposParams;

void setpos(int x, int y)
{
    SetposParams *setposParams = malloc(sizeof (SetposParams));
    setposParams->x = x, setposParams->y = y;
    PostCommand(__setpos, setposParams, false);
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

    MoveParams *moveParams = malloc(sizeof (MoveParams));
    moveParams->dest.x = newPos.x, moveParams->dest.y = newPos.y;
    moveParams->isPenUp = true;
    PostCommand(__move, moveParams, true);
}


typedef struct
{
    int r;
    COLORREF color;
} CircleParams;

void circle(int r)
{
    CircleParams *circleParams = malloc(sizeof (CircleParams));
    circleParams->r = r;
    circleParams->color = t->color;
    PostCommand(__circle, circleParams, false);
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

    HPEN hPen = CreatePen(PS_SOLID, 1, circleParams->color);
    SelectObject(t->hdc, hPen);

    HBRUSH hBrush = CreateSolidBrush(circleParams->color);
    SelectObject(t->hdc, hBrush);

    Ellipse(t->hdc, rect.left, rect.top, rect.right, rect.bottom);

    // clean-up
}

typedef struct
{
    const char *szColor;
} ColorParams;

void color(const char *szColor)
{
     ColorParams *colorParams = malloc(sizeof (ColorParams));
     colorParams->szColor = szColor;
     PostCommand(__color, colorParams, false);
}

static void __color(void *params)
{
    ColorParams *colorParams = (ColorParams *) params;

    // handle uppercase chars

    COLORREF color; 
    if (!strncmp(colorParams->szColor, "white", 5))
        color = RGB(255, 255, 255);
    else if (!strncmp(colorParams->szColo r, "black", 5))
        color = RGB(0, 0, 0);
    else if (!strncmp(colorParams->szColor, "red", 3))
        color = RGB(255, 0, 0);
    else if (!strncmp(colorParams->szColor, "green", 5))
        color = RGB(0, 255, 0);
    else if (!strncmp(colorParams->szColor, "blue", 4))
        color = RGB(0, 0, 255);
    else if (!strncmp(colorParams->szColor, "yellow", 6))
        color = RGB(255, 255, 0);
    else
        return;

    /* HPEN hPen = CreatePen(PS_SOLID, 1, color);
    SelectObject(t->hdc, hPen);

    HBRUSH hBrush = CreateSolidBrush(color);
    SelectObject(t->hdc, hBrush); 

    // delete it */

    t->color = color;
}