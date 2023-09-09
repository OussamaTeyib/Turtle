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
static void __arc(void *params);
static double NormalizeAngle(double angle);
static POINT RotatePoint(POINT centre, POINT point, double alpha);
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
    int nPts;
    COLORREF fillcolor;
    bool fill;
} PolygonParams;

typedef struct {
    RECT rect;
    POINT start, end;
    bool DrawCounterclockwise;
    int penwidth;
    COLORREF pencolor;
    COLORREF fillcolor;
    bool pendown;
    bool fill;
} ArcParams;

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

static void ExecuteCommands(void)
{
    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(command.params);
    }
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

                polygonParams->nPts = 0;
                for (int k = index; k <= i; k++)
                {
                    if (t->cmdQueue[k].cmd != __move)
                        continue;

                    polygonParams->nPts++;
                } 

                polygonParams->apt = calloc(polygonParams->nPts, sizeof (POINT));
                if (!polygonParams->apt)
                    return;

                for (int k = index, aptCounter = 0; k <= i; k++)
                {
                    if (t->cmdQueue[k].cmd != __move)
                        continue;

                    MoveParams *temp = (MoveParams *) t->cmdQueue[k].params;

                    polygonParams->apt[aptCounter++] = temp->dest;
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

    Polygon(t->hdc, polygonParams->apt, polygonParams->nPts);

    // deselect hBrush before deleting it
    SelectObject(t->hdc, hPrevBrush);

    if (polygonParams->fill)
        DeleteObject(hBrush);
}

static void __arc(void *params)
{
    ArcParams *arcParams = (ArcParams *) params;

    HPEN hPen, hPrevPen;
    if (arcParams->pendown)
        hPen = CreatePen(PS_SOLID, arcParams->penwidth, arcParams->pencolor);
    else
        hPen = GetStockObject(NULL_PEN);

    hPrevPen = SelectObject(t->hdc, hPen);

    // if the arc forms a circle that should be filled
    if (arcParams->fill && arcParams->start.x == arcParams->end.x && arcParams->start.y == arcParams->end.y)
    {
        HBRUSH hBrush, hPrevBrush;
        hBrush = CreateSolidBrush(arcParams->fillcolor);
        hPrevBrush = SelectObject(t->hdc, hBrush);

        Ellipse(t->hdc, arcParams->rect.left, arcParams->rect.top, arcParams->rect.right, arcParams->rect.bottom);

        SelectObject(t->hdc, hPrevBrush);
        DeleteObject(hBrush);
    }
    else
    {
        if (!arcParams->DrawCounterclockwise)
            SetArcDirection(t->hdc, AD_CLOCKWISE);

        Arc(t->hdc, arcParams->rect.left, arcParams->rect.top, arcParams->rect.right, arcParams->rect.bottom, arcParams->start.x, arcParams->start.y, arcParams->end.x, arcParams->end.y);
        MoveToEx(t->hdc, arcParams->end.x, arcParams->end.y, NULL);
        
        if (!arcParams->DrawCounterclockwise)
            SetArcDirection(t->hdc, AD_COUNTERCLOCKWISE);
    }

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

static POINT RotatePoint(POINT centre, POINT point, double alpha)
{
    POINT new_point, delta;

    delta.x = point.x - centre.x;
    delta.y = point.y - centre.y;

    new_point.x = centre.x + round(delta.x * cos(alpha) - delta.y * sin(alpha));
    new_point.y = centre.y + round(delta.x * sin(alpha) + delta.y * cos(alpha));

    return new_point;
}
void forward(int distance)
{
    if (!t)
        init();

    MoveParams *moveParams = malloc(sizeof (MoveParams));
    if (!moveParams)
        return;

    double alpha = t->angle / t->fullcircle * 2 * M_PI;

    moveParams->dest = RotatePoint(t->pos, (POINT) {t->pos.x + distance, t->pos.y}, alpha);

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
    if (!moveParams)
        return;

    double alpha = (t->angle + t->fullcircle / 2) / t->fullcircle * 2 * M_PI;

    moveParams->dest = RotatePoint(t->pos, (POINT) {t->pos.x + distance, t->pos.y}, alpha);

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
    if (!moveParams)
        return;
 
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
    setpos(0, 0);
    setheading(0.0);
}

void arc(int r, double extent)
{
    if (!t)
        init();

    double alpha = t->angle / t->fullcircle * 2 * M_PI;
    // To get the centre, if 'r' is positive, rotate the (t->pos.x, t->pos.y + abs(r)), otherwise rotate the point p(t->pos.x, t->pos.y - abs(r)).
    POINT centre = RotatePoint(t->pos, (POINT) {t->pos.x, t->pos.y + r}, alpha);
     
    double beta = extent / t->fullcircle * 2 * M_PI;
    beta *= (r < 0)? -1: 1;  

    ArcParams *arcParams = malloc(sizeof (ArcParams));
    if (!arcParams)
        return;

    arcParams->rect.left = centre.x - r;
    arcParams->rect.top = centre.y + r;
    arcParams->rect.right = centre.x + r;
    arcParams->rect.bottom = centre.y - r;

    arcParams->start = t->pos;
    arcParams->end = RotatePoint(centre, t->pos, beta);
    // Draw the arc in counterclockwise direction if 'beta' is positive, otherwise in clockwise direction.
    arcParams->DrawCounterclockwise = (beta >= 0.0)? true: false;

    arcParams->penwidth = t->penwidth;
    arcParams->pencolor = t->pencolor;
    arcParams->fillcolor = t->fillcolor;
    arcParams->pendown = t->pendown;
    arcParams->fill = t->fill;

    PostCommand((Command) {__arc, arcParams});

    t->pos = arcParams->end;
    t->angle = NormalizeAngle(t->angle + beta / (2 * M_PI) * t->fullcircle);
}

void circle(int r)
{
    arc(r, t->fullcircle);
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

void setheading(double angle)
{
    if (!t)
        init();

    t->angle = NormalizeAngle(angle);
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