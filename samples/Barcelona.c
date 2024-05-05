#include <turtle.h>

int main(void)
{
    up();
    backward(200);
    down();
    
    width(5);
    begin_fill();

    left(20);
    arc(70, 130); 

    right(80);
    forward(20);

    right(90);
    arc(110, 60);

    right(70);
    arc(130, 60);

    right(70);
    arc(110, 60);

    right(80);
    forward(20);

    right(100);
    arc(70, 132); 

    right(90);
    forward(20);

    seth(-90);
    arc(-155, 90);

    left(45);
    fd(25);

    right(90);
    fd(25);

    seth(180);
    arc(-155, 90);

    left(35);
    fd(20);
    
    color("yellow");
    end_fill();

    show();
    return 0;
}