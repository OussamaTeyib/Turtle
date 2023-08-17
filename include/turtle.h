#ifndef TURTLE_H
#define TURTLE_H

#ifdef __cplusplus
extern "C" {
#endif

void init(void);
void show(void);
void forward(int distance);
void left(double angle);
void right(double angle);
void setheading(double angle);
void setpos(int x, int y);
void home(void);
void circle(int r);
void width(int width);
void penup(void);
void pendown(void);
void begin_fill(void);
void end_fill(void);
void color(const char *color);
void pencolor(const char *color);
void fillcolor(const char *color);

#define fd(distance) forward(distance)
#define goto(x, y) setpos(x, y)
#define setposition(x, y) setpos(x, y)
#define seth(angle) setheading(angle)
#define up() penup()
#define pu() penup()
#define down() pendown()
#define pd() pendown()

#ifdef __cplusplus
}
#endif

#endif