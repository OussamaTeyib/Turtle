#ifndef TURTLE_H
#define TURTLE_H

#include <windows.h>

typedef struct Turtle Turtle;

typedef void (*cmdFunction) (Turtle *, void *);

typedef struct {
    cmdFunction cmd;
    void *params;
} Command;

struct Turtle {
    HWND hwnd;
    HDC hdc;
    Command *cmdQueue;
    int nCmd;
};

void die(char *msg);
void init(Turtle *t);
void done(Turtle *t);
POINT GetCentre(HWND hwnd);
void PostCommand(Turtle *t, cmdFunction cmd, void *params);
void ExecuteForward(Turtle *t, void *params);

typedef struct
{
    int distance;
} ForwardParams;

#define forward(t, distance) \
     do \
     { \
         ForwardParams forwardParams = {distance}; \
         PostCommand(t, ExecuteForward, &forwardParams); \
     } while (0)

#endif