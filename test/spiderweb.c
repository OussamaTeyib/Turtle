#include <turtle.h>

int main(void)
{
    char *colors[] = {"purple", "red", "green", "pink", "orange", "blue", "brown", "cyan", "yellow", "violet"};

    pensize(2);
    for (int i = 0; i < 140; i++)
    {
        color(colors[i % 10]);
        forward(i);
        left(60);
        right(90);
    }

    show();
    return 0;
}