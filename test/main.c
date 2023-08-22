#include <math.h>
#include <turtle.h>

int main(void)
{
    fd(100);
    left(90);
    radians();
    fd(100);
    left(M_PI / 2);
    fd(100);
    degrees();
    left(90);
    fd(100);
    show();
    return 0;
}