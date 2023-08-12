#include <turtle.h>

int main(void)
{
    init();
    show();
    begin_fill();
    fd(100);
    goto(50, 50);
    home();
    end_fill(); 
    return 0;
}