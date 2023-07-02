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
    double angle;
    Command *cmdQueue;
    int nCmd;
    int maxCmd;
};

#define MAXCMDS 50

void die(char *msg);
void init(Turtle *t);
void done(Turtle *t);
void CreateCanavas(Turtle *t);
void SetCentre(Turtle *t);
void PostCommand(Turtle *t, cmdFunction cmd, void *params);
void ExecuteCommands(Turtle *t);
void __forward(Turtle *t, void *params);
void __left(Turtle *t, void *params);
void __goto(Turtle *t, void *params);

typedef struct
{
    int distance;
} ForwardParams;

#define forward(t, distance) \
     do \
     { \
         ForwardParams forwardParams = {distance}; \
         PostCommand(t, __forward, &forwardParams); \
     } while (0)

typedef struct
{
    double angle;
} LeftParams;

#define left(t, angle) \
     do \
     { \
         LeftParams leftParams = {angle}; \
         PostCommand(t, __left, &leftParams); \
     } while (0)

#define right(t, angle) \
     do \
     { \
         LeftParams leftParams = {-angle}; \
         PostCommand(t, __left, &leftParams); \
     } while (0)

typedef struct
{
    int x, y;
} GotoParams;

#define goto(t, x, y) \
     do \
     { \
         GotoParams gotoParams = {x, y}; \
         PostCommand(t, __goto, &gotoParams); \
     } while (0)

#endif