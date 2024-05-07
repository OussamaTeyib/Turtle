#include <turtle.h>

int main(void)
{
    // bgcolor("black");
    char *colors[] = {"orange", "orange", "blue", "blue", "green", "green"};

    pensize(2);
    for (int i = 0; i < 360; i++)
    {
        color(colors[i % 6]);
        width(i / 101);
        forward(i);
        right(59);
    }

    show();
    return 0;
}