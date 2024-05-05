#include <turtle.h>

int main(void)
{
    for (int i = 0; i < 8; i++)
    {
        up();
        goto(-200, -200 + 50 * i);
        down();
	
        for (int j = 0; j < 8; j++)
        {
            if (!((i + j) % 2))
                color("brown");
            else
                color("yellow");

            begin_fill();
            for (int k = 0; k < 4; k++)
            {
                forward(50);
                left(90);
            }
            end_fill();
            up();
            forward(50);
            down();
        }
    }

    show();
    return 0;
}