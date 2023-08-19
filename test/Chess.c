#include <turtle.h>

int main(void)
{
    for (int i = 0; i < 8; i++)
    {
        up();
        goto(-160, -160 + 20 * i);
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