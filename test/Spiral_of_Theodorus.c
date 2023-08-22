#include <turtle.h>
#include <math.h>

char *colors[17] = {"gray", "green", "brown", "silver", "blue", "maroon", "turquoise", "yellow", "cyan", "red", "gold", "purple", "orange", "magenta", "pink", "violet", "chocolate"};

int main(void)
{
    int base = 50, height = 50;
    fd(base);
    double angle = 0.0;

    for (int i = 1; i <= 17; i++)
    {
        color(colors[(i - 1) % 17]);
        begin_fill();
        left(90);
        forward(height);

        Position newPos;
        pos(&newPos);
        goto(0, 0);
        goto(newPos.x, newPos.y);
        end_fill();

        angle += atan(1 / sqrt(i + 1)) / (2 * M_PI) * 360;
        seth(angle);
    }

    show();
    return 0;
}