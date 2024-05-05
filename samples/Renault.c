#include <turtle.h>

int main(void)
{
    up();
    goto(-100, -100);
    down();
    begin_fill();
    for (int i = 0; i < 4; i++)
    {
        forward(200);
        left(90);
    }
    end_fill();

    color("white");
    width(10);

    up();
    goto(12, -80);
    down();
    for (int i = 0; i < 4; i++)
    {
        left(60 + (i == 2? 60: 0));
        forward(90 - (i == 3? 50: 0));         
    }
 
    up();   
    goto(-12, 75);
    down();
    seth(180);
    for (int i = 0; i < 4; i++)
    {
        left(60 + (i == 2? 60: 0));
        forward(90 - (i == 3? 50: 0));         
    }
    
    show();
    return 0;
}