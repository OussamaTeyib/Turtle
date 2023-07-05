#include "turtle.h"

int main(void)
{
    Turtle t;
    init(&t);

    for (int i = 0; i < 4; i++)
    {
        int d = 100 - 40 * (i % 2);
        forward(&t, d);
        left(&t, 90);
    }

    goto(&t, 0, -10);
    forward(&t, 100);

    done(&t);
    return 0;    
}