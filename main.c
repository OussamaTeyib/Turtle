#include <turtle.h>
#include <math.h>

int main(void)
{
    init();

    // speed(5);

    up();
    goto(-150, -100);
    down();
    color("blue");
    begin_fill();
    for (int i = 0; i < 4; i++)
    {
        forward(300 - 100 * (i % 2));
        left(90);
    }
    end_fill();

    up();
    goto(0, 0);
    down();
    color("yellow");
    left(36);
    for (int i = 0; i < 12; i++)
    {
        up();
        goto(50 * cos(i * M_PI / 6), 50 * sin(i * M_PI / 6));
        down();
        begin_fill();
        for (int i = 0; i < 5; i++)
        {
            forward(15);
            left(144);
        }
        end_fill();
    }
	
    // hideturtle();
    // done();
    
    show();
    return 0;    
}