#include "turtle.h"

int main(void)
{
    Turtle t;
    init(&t);
    forward(&t, 100);
    forward(&t, 100);
    done(&t);
    return 0;    
}