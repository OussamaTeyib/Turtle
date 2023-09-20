#include <turtle.h>

int main(void)
{
    char *colors[] = {"red", "magenta", "blue", "cyan", "green", "yellow"};

    pensize(2);
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            color(colors[j]);
            circle(150);
            left(10);
        }
    }

    show();
    return 0;
}