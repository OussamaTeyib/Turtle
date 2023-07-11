#include <turtle.h>

int main(void)
{
    init();
    goto(-150, -100);
    color("red");
    rectangle(300, 200);

    goto(-150, -70);
    color("green");
    rectangle(300, 140);

    goto(0, -10);
    color("yellow");
    circle(50);

    goto(0, 5);
    color("green");
    circle(50);

    goto(20, -5);
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