#ifndef TURTLE_H
#define TURTLE_H

#include <windows.h>

typedef void (*cmdFunction) (void *);

typedef struct {
    cmdFunction cmd;
    void *params;
} Command;

#define MAXCMDS 50

void init(void);
void show(void);
void PostCommand(cmdFunction cmd, void *params);
void __forward(void *params);
void __left(void *params);
void __goto(void *params);
void __circle(void *params);
void __color(void *params);


typedef struct
{
    int distance;
} ForwardParams;

#define forward(distance) \
     do \
     { \
         ForwardParams forwardParams = {(distance)}; \
         PostCommand(__forward, &forwardParams); \
     } while (0)

typedef struct
{
    double angle;
} LeftParams;

#define left(angle) \
     do \
     { \
         LeftParams leftParams = {(angle)}; \
         PostCommand(__left, &leftParams); \
     } while (0)

#define right(angle) \
     do \
     { \
         LeftParams leftParams = {(-angle)}; \
         PostCommand(__left, &leftParams); \
     } while (0)

typedef struct
{
    int x, y;
} GotoParams;

#define goto(x, y) \
     do \
     { \
         GotoParams gotoParams = {(x), (y)}; \
         PostCommand(__goto, &gotoParams); \
     } while (0)

typedef struct
{
    int r;
} CircleParams;

#define circle(r) \
     do \
     { \
         CircleParams circleParams = {(r)}; \
         PostCommand(__circle, &circleParams); \
     } while (0)

typedef struct
{
    COLORREF color;
} ColorParams;

#define color(color) \
     do \
     { \
         ColorParams colorParams = {(color)}; \
         PostCommand(__color, &colorParams); \
     } while (0)

#endif