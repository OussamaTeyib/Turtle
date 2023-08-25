#include <turtle.h>

int main(void)
{
    up();
    goto(-120, -90);
    down();

    char *colors[] = {"black", "white", "green"};

    for (int i = 0; i < 3; i++)
    {
        color(colors[i % 3]);
        begin_fill();
        for (int j = 0; j < 4; j++)
        {
            forward(240 - (60  * (j % 2) * (i + 1)));
            left(90);
        }
        end_fill();
    }

    goto(-120, -90);
    left(45);
    color("red");
    begin_fill();
    for (int i = 0; i < 2; i++)
    {
        forward(80);
        left(90);
    }
    end_fill();

    // hideturtle()
    // mainloop()

    show();
    return 0;
}