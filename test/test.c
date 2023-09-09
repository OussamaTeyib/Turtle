#include <math.h>
#include <turtle.h>

int main(void)
{
    color("red");
    
    fd(50);
    left(60);
    fd(50);
    bk(50);

    arc(50, 210);
    fd(50);

    show();
    return 0;
}