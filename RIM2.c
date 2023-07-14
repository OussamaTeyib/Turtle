#include <turtle.h>

int main(void)
{
    init();
    goto(-150, -100);
    color("red");
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 100 * (i % 2));
        left(90);
    }

    goto(-150, -70);
    color("green");
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 160 * (i % 2));
        left(90);
    }

    /* goto(0, -10);
    color("yellow");
    circle(50);

    goto(0, 5);
    color("white");
    circle(50); */

    goto(18, -5);
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