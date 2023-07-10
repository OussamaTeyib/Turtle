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
    rectangle(300, 200);

    goto(-150, -70);
    color(GREEN);
    rectangle(300, 140);

    goto(0, -10);
    color(YELLOW);
    circle(50);

    goto(0, 5);
    color(GREEN);
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