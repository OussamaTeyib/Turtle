#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include <turtle.h>
#include <turtle_colors.h>

#define MAX 0xFFFF

typedef void (*cmdFunction) (void *);

typedef struct {
    cmdFunction cmd;
    void *params;
} Command;

typedef struct {
    double x, y;
} FPOINT;

static void init(void);
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static void CreateCanvas(void);
static void cleanup(void);
static void PostCommand(Command command);
static void ExecuteCommands(void);
static void AddStepToPath(POINT step);
static void AddLineToPath(POINT start, POINT end);
static void AddArcToPath(FPOINT centre, FPOINT start, double alpha, bool drawInCounterclockwiseDirection);
static void MakePolygon(void);
static void __move(void *params);
static void __polygon(void *params);
static void __arc(void *params);
static double NormalizeAngle(double angle);
static FPOINT RotatePoint(FPOINT centre, FPOINT point, double alpha, bool inCounterclockwiseDirection);
static POINT ToStdCoord(FPOINT pt);
static COLORREF GetColor(const char *szColor);

typedef struct {
    HWND hwnd;
    HDC hdc;
    FPOINT pos;
    double angle;
    double fullcircle;
    int penwidth;
    COLORREF pencolor;
    COLORREF fillcolor;
    bool pendown;
    bool fill;
    POINT *path;
    int nStp;
    int maxStp;
    Command *cmdQueue;
    int nCmd;
    int maxCmd;
} Turtle;  

typedef struct
{ 
    POINT dest;
    int penwidth;
    COLORREF color;
    bool pendown;
} MoveParams;

typedef struct
{ 
    POINT *apt;
    int nPts;
    COLORREF color;
} PolygonParams;

typedef struct {
    RECT rect;
    POINT start, end;
    bool DrawCounterclockwise;
    int penwidth;
    COLORREF color;
    bool pendown;
} ArcParams;

static Turtle *t = NULL;

static void init(void)
{
    t = malloc(sizeof (Turtle));
    if (!t)
        return;

    t->hwnd = NULL;
    t->hdc = NULL;
    t->pos = (FPOINT) {0.0, 0.0};
    t->angle = 0.0;
    t->fullcircle = 360.0;
    t->penwidth = 1;
    t->pencolor = BLACK;
    t->fillcolor = BLACK;
    t->pendown = true;
    t->fill = false;
    t->path = NULL;
    t->nStp = 0;
    t->maxStp = 0;

    t->maxCmd = MAX;
    t->cmdQueue = calloc(t->maxCmd, sizeof (Command));
    if (!t->cmdQueue)
        return;

    t->nCmd = 0;
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

    // if end_fill() never called
    if (t->fill)
    {
        free(t->path);
        t->path = NULL;
    }

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

static void ExecuteCommands(void)
{
    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(command.params);
    }
}

static void AddStepToPath(POINT step)
{
    if (!t || !t->path)
       return;

    if (t->nStp == t->maxStp)
    {
        t->maxStp *= 2;
        POINT *temp = realloc(t->path, t->maxStp * sizeof (POINT));
        if (!temp)
            return;

        t->path = temp;
    }
    
    t->path[t->nStp++] = step;
}

static void __move(void *params)
{
    MoveParams *moveParams = (MoveParams *) params;

    HPEN hPen, hPrevPen;
    if (moveParams->pendown)
        hPen = CreatePen(PS_SOLID, moveParams->penwidth, moveParams->color);
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
    hBrush = CreateSolidBrush(polygonParams->color);  

    hPrevBrush = SelectObject(t->hdc, hBrush);
    SelectObject(t->hdc, GetStockObject(NULL_PEN));
    SetPolyFillMode(t->hdc, WINDING);

    Polygon(t->hdc, polygonParams->apt, polygonParams->nPts);

    // deselect hBrush before deleting it
    SelectObject(t->hdc, hPrevBrush);

    DeleteObject(hBrush);
}

static void __arc(void *params)
{
    ArcParams *arcParams = (ArcParams *) params;

    HPEN hPen, hPrevPen;
    if (arcParams->pendown)
        hPen = CreatePen(PS_SOLID, arcParams->penwidth, arcParams->color);
    else
        hPen = GetStockObject(NULL_PEN);

    hPrevPen = SelectObject(t->hdc, hPen);

    if (!arcParams->DrawCounterclockwise)
        SetArcDirection(t->hdc, AD_CLOCKWISE);

    Arc(t->hdc, arcParams->rect.left, arcParams->rect.top, arcParams->rect.right, arcParams->rect.bottom, arcParams->start.x, arcParams->start.y, arcParams->end.x, arcParams->end.y);
    MoveToEx(t->hdc, arcParams->end.x, arcParams->end.y, NULL);
        
    if (!arcParams->DrawCounterclockwise)
        SetArcDirection(t->hdc, AD_COUNTERCLOCKWISE);

    SelectObject(t->hdc, hPrevPen);
    if (arcParams->pendown)
        DeleteObject(hPen);
}

static double NormalizeAngle(double angle)
{
    double new_angle = fmod(angle, t->fullcircle);
    if (new_angle < 0.0)
        new_angle += t->fullcircle;
    return new_angle;
}

static FPOINT RotatePoint(FPOINT centre, FPOINT pt, double alpha, bool inCounterclockwiseDirection)
{
    FPOINT new_pt, delta;

    delta.x = pt.x - centre.x;
    delta.y = pt.y - centre.y;
    
    if (inCounterclockwiseDirection)
    {
        new_pt.x = centre.x + delta.x * cos(alpha) - delta.y * sin(alpha);
        new_pt.y = centre.y + delta.x * sin(alpha) + delta.y * cos(alpha);
    }
    else
    {
        new_pt.x = centre.x + delta.x * cos(alpha) + delta.y * sin(alpha);
        new_pt.y = centre.y - delta.x * sin(alpha) + delta.y * cos(alpha);
    }

    return new_pt;
}

static POINT ToStdCoord(FPOINT pt)
{
    POINT std_pt;
    std_pt.x = round(pt.x);
    std_pt.y = round(pt.y);
    return std_pt;
}

static void AddLineToPath(POINT start, POINT end)
{
    if (!t || !t->path)
        return;

    POINT delta, step, curPt;;
    delta.x = abs(end.x - start.x);
    delta.y = abs(end.y - start.y);

    step.x = (start.x < end.x)? 1: -1;
    step.y = (start.y < end.y)? 1: -1;

    LONG err = delta.x - delta.y;

    curPt = start;
    while (1)
    {
        AddStepToPath(curPt);

        if (curPt.x == end.x && curPt.y == end.y)
            break;

        LONG e2 = 2 * err;
        if (e2 > -delta.y)
        {
            err -= delta.y;
            curPt.x += step.x;
        }
               
        if (e2 < delta.x)
        {
            err += delta.x;
            curPt.y += step.y;
        }
    }
}

static void AddArcToPath(FPOINT centre, FPOINT start, double beta, bool drawInCounterclockwiseDirection)
{
    if (beta >= 0)
    {
        for (double i = 0; i <= beta; i += 2 * M_PI / 360)
        {
            POINT pt = ToStdCoord(RotatePoint(centre, start, i, drawInCounterclockwiseDirection));
            AddStepToPath(pt);   
        }
    }
    else
    {
        for (double i = 0; i >= beta; i -= 2 * M_PI / 360)
        {
            POINT pt = ToStdCoord(RotatePoint(centre, start, i, drawInCounterclockwiseDirection));
            AddStepToPath(pt);   
        }
    }
}

void forward(double distance)
{
    if (!t)
        init();

    MoveParams *moveParams = malloc(sizeof (MoveParams));
    if (!moveParams)
        return;

    double alpha = t->angle / t->fullcircle * 2 * M_PI;
    FPOINT new_pos = RotatePoint(t->pos, (FPOINT) {t->pos.x + distance, t->pos.y}, alpha, true);
    moveParams->dest = ToStdCoord(new_pos);

    moveParams->penwidth = t->penwidth;
    moveParams->color = t->pencolor;
    moveParams->pendown = t->pendown;
    
    PostCommand((Command) {__move, moveParams});

    if (t->fill)
        AddLineToPath(ToStdCoord(t->pos), moveParams->dest);
 
    t->pos = new_pos;
}

void backward(double distance)
{
    if (!t)
        init();

    MoveParams *moveParams = malloc(sizeof (MoveParams));
    if (!moveParams)
        return;

    double alpha = (t->angle + t->fullcircle / 2) / t->fullcircle * 2 * M_PI;
    FPOINT new_pos = RotatePoint(t->pos, (FPOINT) {t->pos.x + distance, t->pos.y}, alpha, true);
    moveParams->dest = ToStdCoord(new_pos);

    moveParams->penwidth = t->penwidth;
    moveParams->color = t->pencolor;
    moveParams->pendown = t->pendown;
    
    PostCommand((Command) {__move, moveParams});

    if (t->fill)
        AddLineToPath(ToStdCoord(t->pos), moveParams->dest);
 
    t->pos = new_pos;
}

void setposition(double x, double y)
{
    if (!t)
        init();

    MoveParams *moveParams = malloc(sizeof (MoveParams));
    if (!moveParams)
        return;
 
    moveParams->dest = ToStdCoord((FPOINT) {x, y});
    moveParams->penwidth = t->penwidth;
    moveParams->color = t->pencolor;
    moveParams->pendown = t->pendown;
    
    PostCommand((Command) {__move, moveParams});

    if (t->fill)
        AddLineToPath(ToStdCoord(t->pos), moveParams->dest);

    t->pos = (FPOINT) {x, y};
}

void home(void)
{
    setposition(0.0, 0.0);
    setheading(0.0);
}

void arc(double r, double extent)
{
    if (!t)
        init();

    double alpha = t->angle / t->fullcircle * 2 * M_PI;
    // To get the centre, if 'r' is positive, rotate the (t->pos.x, t->pos.y + abs(r)), otherwise rotate the point p(t->pos.x, t->pos.y - abs(r)).
    FPOINT centre = RotatePoint(t->pos, (FPOINT) {t->pos.x, t->pos.y + r}, alpha, true);
     
    double beta = extent / t->fullcircle * 2 * M_PI; 

    ArcParams *arcParams = malloc(sizeof (ArcParams));
    if (!arcParams)
        return;

    arcParams->rect.left = round(centre.x - r);
    arcParams->rect.top = round(centre.y + r);
    arcParams->rect.right = round(centre.x + r);
    arcParams->rect.bottom = round(centre.y - r);

    arcParams->start = ToStdCoord(t->pos);
    FPOINT end = RotatePoint(centre, t->pos, beta, (r > 0.0)? true: false);
    arcParams->end = ToStdCoord(end);
    // Draw the arc in counterclockwise direction if both 'r' and 'beta' have the same sign, otherwise in clockwise direction.
    arcParams->DrawCounterclockwise = (r * beta > 0.0)? true: false;

    arcParams->penwidth = t->penwidth;
    arcParams->color = t->pencolor;
    arcParams->pendown = t->pendown;

    PostCommand((Command) {__arc, arcParams});

    if (t->fill)
        AddArcToPath(centre, t->pos, beta, (r > 0.0)? true: false);
 
    t->pos = end;
    double new_angle = beta * ((r < 0.0)? -1: 1);
    t->angle = NormalizeAngle(t->angle + new_angle / (2 * M_PI) * t->fullcircle);
}

void circle(double r)
{
    arc(r, t->fullcircle);
}

static void MakePolygon(void)
{
    if (!t || !t->path)
       return;

    PolygonParams *polygonParams = malloc(sizeof (PolygonParams));
    if (!polygonParams)
        return;

    polygonParams->nPts = t->nStp;

    polygonParams->apt = calloc(polygonParams->nPts, sizeof (POINT));
    if (!polygonParams->apt)
        return;

    for (int i = 0; i < t->nStp; i++)
        polygonParams->apt[i] = t->path[i];

    polygonParams->color = t->fillcolor;

    PostCommand((Command) {__polygon, polygonParams});
}

void degrees(void)
{
    fullcircle(360.0);
}

void radians(void)
{
    fullcircle(2 * M_PI);
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

    t->angle = NormalizeAngle(t->angle + angle);
}

void right(double angle)
{   
    left(-angle);
}

void setheading(double to_angle)
{
    if (!t)
        init();

    t->angle = NormalizeAngle(to_angle);
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
    
    if (t->path)
    {
        free(t->path);
        t->path = NULL;
    }

    t->maxStp = MAX;
    t->path = malloc(t->maxStp * sizeof (POINT));
    t->nStp = 0;
}

void end_fill(void)
{
    if (!t)
        init();

    t->fill = false;

    if (t->path)
    {
        MakePolygon();
        t->maxStp = t->nStp = 0; 
        free(t->path);
        t->path = NULL;
    }
}

TurtlePosition position(void)
{
    if (!t)
        init();

    return (TurtlePosition) {t->pos.x, t->pos.y};
}

double heading(void)
{
    if (!t)
        init();

    return t->angle;
}