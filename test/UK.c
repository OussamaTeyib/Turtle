#include <turtle.h>

int main(void)
{
    up();
    goto(-150, -100);
    down();
    color("blue");
    begin_fill();
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 100 * (i % 2));
        left(90);
    }
    end_fill();

    color("white");
    width(40);
    goto(150, 100);
    up();
    goto(-150, 100);
    down();
    goto(150, -100);

    color("red");
    width(20);
    up();
    goto(150, -90);
    down();
    goto(0, 10);
    goto(135, 100);

    up();
    goto(-150, 90);
    down();
    goto(0, -10);
    goto(-135, -100);

    color("white");
    width(40);
    up();
    goto(0, -100);
    down();
    goto(0, 100);
    up();
    goto(-150, 0);
    down();
    goto(150, 0);

    color("red");
    width(25);
    goto(-150, 0);
    up();
    goto(0, -100);
    down();
    goto(0, 100);

    color("white");
    width(30);
    up();
    goto(-165, -115);
    down();
    for (int i = 0; i < 4; i++)
    {
        forward(330 - 100 * (i % 2));
        left(90);
    }

    show();
    return 0;
}