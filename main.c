#include "turtle.h"

int main(void)
{
    init();
    goto(0, 0);
    for (int i = 0; i < 4; i++)
    {
        forward(200 - 40 * (i % 2));
        left(90);
    }
    show();
    return 0;    
}