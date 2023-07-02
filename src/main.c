#include "turtle.h"

int main(void)
{
    Turtle t;
    init(&t);
    forward(&t, 100);
    left(&t, 90);
    forward(&t, 40);
    left(&t, 90);
    forward(&t, 100);
    left(&t, 90);
    forward(&t, 40);

    left(&t, 90);
    goto (&t, 25, -25);
    forward(&t, 100);
    right(&t, 90);
    forward(&t, 40);
    right(&t, 90);
    forward(&t, 100);
    right(&t, 90);
    forward(&t, 40);

    done(&t);
    return 0;    
}