#include <turtle.h>

int main(void)
{
    init();
    up();
    goto(-150, -100);
    down();
    color("red");
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 100 * (i % 2));
        left(90);
    }

    up();
    goto(-150, -70);
    down();
    color("green");
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 160 * (i % 2));
        left(90);
    }

    up();
    goto(0, -10);
    down();
    color("yellow");
    begin_fill();
    circle(50);
    end_fill();

    up();
    goto(0, 5);
    down();
    color("white");
    begin_fill();
    circle(50);
    end_fill();;

    up();
    goto(18, -5);
    down();
    left(108);
    color("yellow");
    for (int i = 0; i < 5; i++)
    {
        forward(60);
        left(144);
    }

    show();
    return 0;
}