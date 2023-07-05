#include "turtle.h"

int main(void)
{
    Turtle t;
    init(&t);

    goto(&t, -150, -50);
    for (int i = 0; i < 4; i++)
    {
        forward(&t, 300 - 100 * (i % 2));
        left(&t, 90); 
    }

    goto(&t, -150, -20);
    for (int i = 0; i < 3; i++)
    {
        forward(&t, 300 -160 * (i%2));
        left(&t, 90);
    }

    goto(&t, -20, 40);
    left(&t, 126);
    for (int i = 0; i < 5; i++)
    {
        forward(&t, 60);
        left(&t, 144);
    }
    
    done(&t);
    return 0;
}