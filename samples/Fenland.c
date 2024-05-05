#include <turtle.h>

int main(void)
{
    up();
    goto(-150, -100);
    down();
    color("blue");
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 100 * (i % 2));
        left(90);
    }

    up();
    goto(-60, -100);
    down();
    begin_fill();
    for (int i = 0; i < 4; i++)
    {
        forward(50 + 150 * (i % 2));
        left(90);
    }
    end_fill();

    up();
    goto(-150, -25);
    down();
    begin_fill();
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 250 * (i % 2));
        left(90);
    }
    end_fill();

    show();
    return 0;
}