#include "turtle.h"
#include <math.h>

void die(char *msg)
{
    MessageBox(NULL, msg, "ERROR", MB_ICONEXCLAMATION | MB_OK);    
    exit(EXIT_FAILURE);
}
 
void init(Turtle *t)
{
    t->angle = 0;

    t->maxCmd = MAXCMDS;
    t->cmdQueue = malloc(t->maxCmd * sizeof (Command));
    t->nCmd = 0;
}

void CreateCanavas(Turtle *t)
{
    char *className = "MainWindow";
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {0};
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszClassName = className;

    if (!RegisterClass(&wc))
        die("Initialization Failed! \nClass NOT Registred.");

    HWND hwnd = CreateWindow(className, "Turtle in C", WS_OVERLAPPEDWINDOW, 250, 100, 720, 380, NULL, NULL, hInstance, NULL);

    if (!hwnd)
        die("Initialization Failed! \nWindow NOT Created");

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    t->hwnd = hwnd;
    t->hdc = GetDC(hwnd);
    SetCentre(t);

    ExecuteCommands(t);

    MSG msg;
    while (GetMessage(&msg, t->hwnd, 0, 0) > 0)
    {
        if (msg.message == WM_PAINT)
        {
            SetCentre(t);
            ExecuteCommands(t);
        }
        DispatchMessage(&msg);
    }

    ReleaseDC(t->hwnd, t->hdc);
    UnregisterClass(className, hInstance);
}

void done(Turtle *t)
{ 
    CreateCanavas(t);
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

void ExecuteCommands(Turtle *t)
{
    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(t, command.params);
    }
}

void SetCentre(Turtle *t)
{
    RECT rect;
    GetClientRect(t->hwnd, &rect);
    MoveToEx(t->hdc, rect.right / 2, rect.bottom / 2, NULL);
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
    GotoParams *gotoParams = (gotoParams *) params;

    RECT rect;
    GetClientRect(t->hwnd, &rect);
    POINT newPos;
    newPos.x = rect.right / 2 + gotoParams->x;
    newPos.y = rect.bottom / 2 - gotoParams->y; // '-' because that y-axis increases downward
    MoveToEx(t->hdc, newPos.x, newPos.y, NULL);
}