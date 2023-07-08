#include "turtle.h"
#include <windows.h> // while i am using RGB

int main(void)
{
    init();
    color(RGB(255, 0, 0));
    goto(-150, -100);
    for (int i = 0; i < 4; i++)
    {
        if (i % 2)
           forward(200);
        else
           forward(300);
        left(90);
    }

    color(RGB(0, 255, 0));
    goto(-150, -80);
    for (int i = 0; i < 4; i++)
    {
        if (i % 2)
           forward(160);
        else
           forward(300);
        left(90);
    }

    goto(0, -20);
    color(RGB(255, 255, 0));
    circle(50);

    color(RGB(255, 255, 255));
    goto(0, -5);
    circle(50);

    color(RGB(255, 255, 0));
    goto(20, -10);
    left(108);
    for (int i = 0; i < 5; i++)
    {
        forward(60);
        left(144);
    }

    show();
    return 0;    
}