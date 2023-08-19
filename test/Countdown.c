#define _USE_MATH_DEFINES

#include <turtle.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>

void watch(int n)
{
    for (int i = n; i > 0; i--)
    {
        width(5);
        color("white");
        goto(140 * cos(i * M_PI / 30), 140 * sin(i * M_PI / 30));
        // hideturtle();
        // Sleep(1);

        // if (i)
        //    color("black");

        goto(0, 0);
    }
}

int main(int argc, char *argv[])
{
    // speed(0);

    // draw the red circle
    up();
    goto(0, -150);
    down();
    color("red");
    begin_fill();
    circle(150);
    end_fill();

    // draw the chronometer
    up();
    goto(0, 0);
    down();
    for (int i = 0; i < 60; i++)
    {
        width(5);
        color("black");
        goto(140 * cos(i * M_PI / 30), 140 * sin(i * M_PI /30));
        // hideturtle();
        goto(0, 0);
    }

    if (argc > 1)
    {
        watch(atoi(argv[1]));
    }

    show();
    return 0;
}