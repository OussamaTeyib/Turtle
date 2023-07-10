#ifndef TURTLE_H
#define TURTLE_H

void init(void);
void show(void);
void forward(int distance);
void left(double angle);
void right(double angle);
void setpos(int x, int y);
void circle(int r);
void color(int color);
void rectangle(int length, int width);

#define fd(distance) forward((distance))
#define goto(x, y) setpos((x), (y))

#endif