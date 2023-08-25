#include <turtle.h>
#include <math.h>

char *colors[17] = {"gray", "green", "brown", "silver", "blue", "maroon", "turquoise", "yellow", "cyan", "red", "gold", "purple", "orange", "magenta", "pink", "violet", "chocolate"};

int main(void)
{
    int distance = 50;
    double angle = 0.0;

    for (int i = 0; i < 17; i++)
    {
        color(colors[i % 17]);

        begin_fill();
        fd(distance * sqrt(i + 1));
        left(90);
        forward(distance);
        goto(0, 0);
        end_fill();

        angle += atan(1 / sqrt(i + 1)) / (2 * M_PI) * 360;
        seth(angle);
    }

    show();
    return 0;
}