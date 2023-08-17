#include <turtle.h>

int main(void)
{	
    init();
    for (int i = 0; i < 2; i++)
    {
        up();
        goto(-160, -160 + 20 * i);
        down();
        for (int j = 0; j < 2; j++)
        {
            if (!((i + j) % 2))
                color("green");
            else
                color("white");

            begin_fill();
            for (int k = 0; k < 4; k++)
            {
                forward(20);
                left(90);
            }
            end_fill();
            forward(20);
        }
    }

    show();
    return 0;
}