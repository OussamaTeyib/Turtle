#include <turtle.h>

int main(void)
{
    arc(50, 180);
    arc(100, 180);
    arc(50, -180);
    
    seth(0);
    up();
    goto(0, 35);
    down();
    begin_fill();
    circle(15);
    end_fill();
    
    up();
    home();
    down();
    seth(180);
    begin_fill();
    arc(50, 180);
    arc(100, 180);
    arc(50, -180);
    end_fill();

    seth(0);
    up();
    goto(0, -65);
    down();
    color("white");
    begin_fill();
    circle(15);
    end_fill();


    show();
    return 0;
}