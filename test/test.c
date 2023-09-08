#include <math.h>
#include <turtle.h>

int main(void)
{
    color("red");
    
    
    fd(50);
    // circle(-50);

    bk(50);
    seth(-45);
    fd(50);    
    circle(-50);

    show();
    return 0;
}