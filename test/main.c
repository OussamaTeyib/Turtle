#include <math.h>
#include <turtle.h>

int main(void)
{
    color("red");
    fd(100);
    seth(90);
    fd(50);
    bk(50);
    seth(270);
    circle(50);
    left(90);
    fd(50);
    show();
    return 0;
}