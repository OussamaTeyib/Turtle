#include <turtle.h>
#include <windows.h>

#define WHITE RGB(255, 255, 255)
#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 255, 0)
#define YELLOW RGB(255, 255, 0)

int main(void)
{
    init();
    goto(-30, -25);
    color(RED);
    for (int i = 0; i < 4; i++)
    {
        forward(60 - 10 * (i % 2));
        left(90);
    }

    goto(-30, -15);
    color(GREEN);
    for (int i = 0; i < 4; i++)
    {
        forward(60 - 30 * (i % 2));
        left(90);
    }

    show();
    return 0;
}