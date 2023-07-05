#include "turtle.h"
#include <windows.h>

int main(void)
{
    Turtle t;
    init(&t);

    color(&t, RGB(255, 0, 0));
    goto(&t, -150, -100);
    for (int i = 0; i < 4; i++)
    {
        if (i % 2)
           forward(&t, 200);
        else
           forward(&t, 300);
        left(&t, 90);
    }

    color(&t, RGB(0, 255, 0));
    goto(&t, -150, -80);
    for (int i = 0; i < 4; i++)
    {
        if (i % 2)
           forward(&t, 160);
        else
           forward(&t, 300);
        left(&t, 90);
    }

    goto(&t, 0, -20);
    color(&t, RGB(255, 255, 0));
    circle(&t, 50);

    color(&t, RGB(255, 255, 255));
    goto(&t, 0, -5);
    circle(&t, 50);

    color(&t, RGB(255, 255, 0));
    goto(&t, 20, -10);
    left(&t, 108);
    for (int i = 0; i < 5; i++)
    {
        forward(&t, 60);
        left(&t, 144);
    }

    done(&t);
    return 0;    
}