#include "turtle.h"
#include <windows.h>

int main(void)
{
    init();
    color(RGB(255, 255, 0));
    forward(100);
    left(90);
    forward(60);
    left(90);
    forward(100);
    left(90);
    forward(60);


    show();
    return 0;
}