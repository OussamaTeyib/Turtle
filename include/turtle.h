#ifndef TURTLE_H
#define TURTLE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x, y;
} Position;
 
void show(void);
void forward(int distance);
void backward(int distance);
void setpos(int x, int y);
void home(void);
void degrees(void);
void radians(void);
void fullcircle(double units);
void left(double angle);
void right(double angle);
void setheading(double angle);
void circle(int r);
void arc(int r, double extent);
void width(int width);
void penup(void);
void pendown(void);
void begin_fill(void);
void end_fill(void);
void color(const char *color);
void pencolor(const char *color);
void fillcolor(const char *color);
void pos(Position *position);
double heading(void);

#define fd(distance) forward(distance)
#define bk(distance) backward(distance)
#define back(distance) backward(distance)
#define goto(x, y) setpos(x, y)
#define setposition(x, y) setpos(x, y)
#define seth(angle) setheading(angle)
#define up() penup()
#define pu() penup()
#define down() pendown()
#define pd() pendown()

// #define pensize(width) width(width)
// #define position(p) pos(p)

#ifdef __cplusplus
}
#endif

#endif