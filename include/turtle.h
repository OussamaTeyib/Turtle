#ifndef TURTLE_H
#define TURTLE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double x, y;
} TurtlePosition;
 
void show(void);

void forward(double distance);
void backward(double distance);
void setposition(double x, double y);
void home(void);
TurtlePosition position(void);

void left(double angle);
void right(double angle);
double heading(void);
void setheading(double to_angle);
void degrees(void);
void radians(void);
void fullcircle(double units);

void circle(double r);
void arc(double r, double extent);

void width(int width);
void penup(void);
void pendown(void);

void begin_fill(void);
void end_fill(void);

void color(const char *color);
void pencolor(const char *color);
void fillcolor(const char *color);
void bgcolor(const char *color);


#define fd(distance) forward(distance)
#define bk(distance) backward(distance)
#define back(distance) backward(distance)
#define goto(x, y) setposition(x, y)
#define setpos(x, y) setposition(x, y)
#define pos() position()

#define lt(angle) left(angle)
#define rt(angle) right(angle)
#define seth(to_angle) setheading(to_angle)

#define up() penup()
#define pu() penup()
#define down() pendown()
#define pd() pendown()
#define pensize(w) width(w)

#ifdef __cplusplus
}
#endif

#endif