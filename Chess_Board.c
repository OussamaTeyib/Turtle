#include <turtle.h>

int main(void)
{	
    init();
    for (int i = 0; i < 8; i++)
    {
        up();
        goto(-320, -320 + 80 * i);
        down();
        for (int j = 0; j < 8; j++)
        {
            if (!((i + j) % 2))
                color("green");
            else
                color("white");

            begin_fill();
            for (int k = 0; k < 4; k++)
            {
                forward(80);
                left(90);
            }
            forward(80);
            end_fill();
        }
    }

    show();
    return 0;
}