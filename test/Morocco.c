#include <turtle.h>

int main(void)
{
    up();
    goto(-150, -100);
    down();
    color("red");
    begin_fill();
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 100 * (i % 2));
        left(90);
    }
    end_fill();

    up();
    goto(-40, -60);
    down();
    color("green");
    width(7);
    left(36);
    for (int i = 0; i < 5; i++)
    {
        forward(130);
        left(180 - 36);
    }

    show();
    return 0;
}