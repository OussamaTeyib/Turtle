#include <turtle.h>
#include <windows.h>

#define WHITE RGB(255, 255, 255)
#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 255, 0)
#define YELLOW RGB(255, 255, 0)

int main(void)
{
    init();
    goto(-150, -100);
    color(RED);
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 100 * (i % 2));
        left(90);
    }

    goto(-150, -70);
    color(GREEN);
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 160 * (i % 2));
        left(90);
    }

    goto(0, -10);
    color(YELLOW);
    circle(50);

    goto(0, 5);
    color(WHITE);
    circle(50);

    goto(20, -5);
    left(108);
    color(YELLOW);
    for (int i = 0; i < 5; i++)
    {
        forward(60);
        left(144);
    }

    show();
    return 0;
}