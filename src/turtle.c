#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include <turtle.h>
#include <turtle_colors.h>

#define MAX_CMDS 100

typedef void (*cmdFunction) (void *);

typedef struct {
    cmdFunction cmd;
    void *params;
} Command;

static void init(void);
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void CreateCanvas(void);
static void cleanup(void);
static void PostCommand(Command command);
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
    double fullcircle;
    int penwidth;
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
    int penwidth;
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
    POINT pos;
    int r;
    int penwidth;
    COLORREF pencolor;
    COLORREF fillcolor;
    bool pendown;
    bool fill;
} CircleParams;

static Turtle *t = NULL;

static void init(void)
{
    t = malloc(sizeof (Turtle));
    if (!t)
        return;

    t->hwnd = NULL;
    t->hdc = NULL;
    t->pos = (POINT) {0, 0};
    t->angle = 0.0;
    t->fullcircle = 360.0;
    t->penwidth = 1;
    t->pencolor = BLACK;
    t->fillcolor = BLACK;
    t->pendown = true;
    t->fill = false;

    t->maxCmd = MAX_CMDS;
    t->cmdQueue = calloc(t->maxCmd, sizeof (Command));
    if (!t->cmdQueue)
        return;

    t->nCmd = 0;
    // add goto(0, 0) to the cmdQueue to improve polygon detection
    MoveParams *initMove = malloc(sizeof (MoveParams));
    initMove->dest = (POINT) {0, 0};
    initMove->pendown = false;
    PostCommand((Command) {__move, initMove});
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
    if (!t)
       init();

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

static void PostCommand(Command command)
{
    if (!t || !t->cmdQueue)
       return;

    if (t->nCmd == t->maxCmd)
    {
        t->maxCmd *= 2;
        Command *temp = realloc(t->cmdQueue, t->maxCmd * sizeof (Command));
        if (!temp)
            return;

        t->cmdQueue = temp;
    }
    
    t->cmdQueue[t->nCmd++] = command;
}

static void LinesToPolygon(void)
{
    // skip if there is only the initial move in the cmdQueue
    if (1 == t->nCmd)
       return;

    int maxCmd = t->maxCmd;
    Command *cmdQueue = calloc(maxCmd, sizeof (Command));
    if (!cmdQueue)
       return;

    int nCmd = 0;

    for (int i = 0; i < t->nCmd; i++)
    {
        if (nCmd == maxCmd)
        {
            maxCmd *= 2;
            Command *temp = realloc(cmdQueue, maxCmd * sizeof (Command));
            if (!temp)
                return;

            cmdQueue = temp;
        } 
        cmdQueue[nCmd++] = t->cmdQueue[i];

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

                if (nCmd == maxCmd)
                {
                    maxCmd *= 2;
                    Command *temp = realloc(cmdQueue, maxCmd * sizeof (Command));
                    if (!temp)
                    return;

                    cmdQueue = temp;
                } 
                cmdQueue[nCmd++] = (Command) {__polygon, polygonParams};
            }
        }
    }

    free(t->cmdQueue);
    t->cmdQueue = cmdQueue;
    t->nCmd = nCmd;
    t->maxCmd = maxCmd;
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
        hPen = CreatePen(PS_SOLID, moveParams->penwidth, moveParams->pencolor);
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

    RECT rect;
    rect.left = circleParams->pos.x - circleParams->r;
    rect.top = circleParams->pos.y + 2 * circleParams->r;
    rect.right = circleParams->pos.x + circleParams->r;
    rect.bottom = circleParams->pos.y;

    HPEN hPen, hPrevPen;
    if (circleParams->pendown)
        hPen = CreatePen(PS_SOLID, circleParams->penwidth, circleParams->pencolor);
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
    if (!t)
        init();

    MoveParams *moveParams = malloc(sizeof (MoveParams));

    double alpha = t->angle / t->fullcircle * 2 * M_PI;

    moveParams->dest.x = round(t->pos.x + distance * cos(alpha));
    moveParams->dest.y = round(t->pos.y + distance * sin(alpha));

    moveParams->penwidth = t->penwidth;
    moveParams->pencolor = t->pencolor;
    moveParams->fillcolor = t->fillcolor;
    moveParams->pendown = t->pendown;
    moveParams->fill = t->fill;
    
    PostCommand((Command) {__move, moveParams});

    t->pos = moveParams->dest;
}

void backward(int distance)
{
    if (!t)
        init();

    MoveParams *moveParams = malloc(sizeof (MoveParams));

    double alpha = (t->angle + t->fullcircle / 2) / t->fullcircle * 2 * M_PI;

    moveParams->dest.x = round(t->pos.x + distance * cos(alpha));
    moveParams->dest.y = round(t->pos.y + distance * sin(alpha));

    moveParams->penwidth = t->penwidth;
    moveParams->pencolor = t->pencolor;
    moveParams->fillcolor = t->fillcolor;
    moveParams->pendown = t->pendown;
    moveParams->fill = t->fill;
    
    PostCommand((Command) {__move, moveParams});

    t->pos = moveParams->dest;
}

void setpos(int x, int y)
{
    if (!t)
        init();

    MoveParams *moveParams = malloc(sizeof (MoveParams));
 
    moveParams->dest = (POINT) {x, y};
    moveParams->penwidth = t->penwidth;
    moveParams->pencolor = t->pencolor;
    moveParams->fillcolor = t->fillcolor;
    moveParams->pendown = t->pendown;
    moveParams->fill = t->fill;
    
    PostCommand((Command) {__move, moveParams});

    t->pos = moveParams->dest;
}

void home(void)
{
    if (!t)
        init();

    setpos(0, 0);
    setheading(0.0);
}

void degrees(void)
{
    if (!t)
        init();

    t->angle = t->angle / t->fullcircle * 360.0;
    t->fullcircle = 360.0;
}

void radians(void)
{
    if (!t)
        init();

    t->angle = t->angle / t->fullcircle * 2 * M_PI;
    t->fullcircle = 2 * M_PI;
}

void fullcircle(double units)
{
    if (!t)
        init();

    t->angle = t->angle / t->fullcircle * units;
    t->fullcircle = units;
}

void left(double angle)
{
    if (!t)
        init();

    t->angle += angle;
}

void right(double angle)
{   
    left(-angle);
}

void setheading(double angle)
{
    if (!t)
        init();

    t->angle = angle;
}

void width(int width)
{
    if (!t)
        init();

    t->penwidth = width;
}

static COLORREF GetColor(const char *szColor)
{
    if (!strncmp(szColor, "white", strlen("white")))
        return WHITE;
    else if (!strncmp(szColor, "silver", strlen("silver")))
        return SILVER;
    else if (!strncmp(szColor, "gray", strlen("gray")) || !strncmp(szColor, "grey", strlen("grey")))
        return GRAY;
    else if (!strncmp(szColor, "black", strlen("black")))
        return BLACK;
    else if (!strncmp(szColor, "blue", strlen("blue")))
        return BLUE;
    else if (!strncmp(szColor, "cyan", strlen("cyan")))
        return CYAN;
    else if (!strncmp(szColor, "turquoise", strlen("turquoise")))
        return TURQUOISE;
    else if (!strncmp(szColor, "green", strlen("green")))
        return GREEN;
    else if (!strncmp(szColor, "yellow", strlen("yellow")))
        return YELLOW;
    else if (!strncmp(szColor, "gold", strlen("gold")))
        return GOLD;
    else if (!strncmp(szColor, "orange", strlen("orange")))
        return ORANGE;
    else if (!strncmp(szColor, "chocolate", strlen("chocolate")))
        return CHOCOLATE;
    else if (!strncmp(szColor, "brown", strlen("brown")))
        return BROWN;
    else if (!strncmp(szColor, "maroon", strlen("maroon")))
        return MAROON;
    else if (!strncmp(szColor, "red", strlen("red")))
        return RED;
    else if (!strncmp(szColor, "pink", strlen("pink")))
        return PINK;
    else if (!strncmp(szColor, "purple", strlen("purple")))
        return PURPLE;
    else if (!strncmp(szColor, "violet", strlen("violet")))
        return VIOLET;
    else if (!strncmp(szColor, "magenta", strlen("magenta")))
        return MAGENTA;
    else if (!strncmp(szColor, "indigo", strlen("indigo")))
        return INDIGO;
    else
        return -1;
}

void color(const char *szColor)
{
    pencolor(szColor);
    fillcolor(szColor); 
}

void pencolor(const char *szColor)
{
    if (!szColor)
        return;

    if (!t)
        init();

    COLORREF color;
    if ((color = GetColor(szColor)) == (COLORREF) -1)
        return;

    t->pencolor = color; 
}

void fillcolor(const char *szColor)
{
    if (!szColor)
        return;

    if (!t)
        init();

    COLORREF color;
    if ((color = GetColor(szColor)) == (COLORREF) -1)
        return;

    t->fillcolor = color; 
}

void penup(void)
{
    if (!t)
        init();

     t->pendown = false;
}

void pendown(void)
{
    if (!t)
        init();

     t->pendown= true;
}

void begin_fill(void)
{
    if (!t)
        init();

     t->fill = true;
}

void end_fill(void)
{
    if (!t)
        init();

     t->fill = false;
}

void circle(int r)
{
    if (!t)
        init();

    CircleParams *circleParams = malloc(sizeof (CircleParams));
    circleParams->pos = t->pos;
    circleParams->r = r;
    circleParams->penwidth = t->penwidth;
    circleParams->pencolor = t->pencolor;
    circleParams->fillcolor = t->fillcolor;
    circleParams->pendown = t->pendown;
    circleParams->fill = t->fill;

    PostCommand((Command) {__circle, circleParams});
}

void pos(Position *position)
{
    if (!position)
        return;

    if (!t)
        init();

    *position = (Position) {t->pos.x, t->pos.y};
}

double heading(void)
{
    if (!t)
        init();

    return t->angle;
}