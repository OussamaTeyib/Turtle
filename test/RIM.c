#include <turtle.h>

int main(void)
{
    init();
    up();
    goto(-150, -100);
    down();
    color("red");
    begin_fill();
    for (int i = 0; i < 4; i++)
    {
        fd(300 - 100 * (i % 2));
        left(90);
    }
    end_fill();

    up();
    goto(-150, -70);
    down();
    color("green");
    begin_fill();
    for (int i = 0; i < 4; i++)
    {
        fd(300 - 160 * (i % 2));
        left(90);
    }
    end_fill();

    up();
    goto(0, -60);
    down();
    color("yellow");
    begin_fill();
    circle(50);
    end_fill();

    up();
    goto(0, -45);
    down();
    color("green");
    begin_fill();
    circle(50);
    end_fill();

    up();
    goto(18, -5);
    down();
    color("yellow");
    left(108);
    begin_fill();
    for (int i = 0; i < 5; i++)
    {
        forward(60);
        left(144);
    }
    end_fill();

    show();
    return 0;    
}