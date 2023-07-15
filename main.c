#include <turtle.h>
#include <math.h>

int main(void)
{
    init();
    begin_fill();
    up();
    goto(-150, -100);
    down();
    color("red");
    for (int i = 0; i < 5; i++)
    {
        forward(300 - 100 * (i % 2));
        left(90);
    }
    end_fill();

    color("green");
    goto(-150, -80);

    /* 
    for (int i = 0; i < 5; i++)
    {
        forward(300 - 140 * (i % 2));
        left(90);
    }
    end_fill(); */

    show();
    return 0;    
}