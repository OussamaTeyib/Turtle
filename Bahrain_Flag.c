#include <turtle.h>
#include <math.h>

int main(void)
{
    init();
    up();
    goto(-150, -100);
    down();
    color("red");
    for(int i = 0; i < 4; i++)
    {
        forward(300 - 100 * (i % 2));
	left(90);
    }

    begin_fill();
    forward(100);
    for(int i = 0; i < 3; i++)
    {
        forward(200);
        left(90);
    }

    left(60);
    for(int i = 0; i < 5; i++)
    {
        for(int i = 0; i < 2; i++)
        {
            forward(40);
            right(120);
        }
        right(120);
    }
    end_fill();

    show();
    return 0;    
}