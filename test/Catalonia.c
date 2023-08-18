#include <turtle.h>

int main(void)
{
    for (int i = 0; i < 5; i++)
    {
        up();
        goto(-200, -153 + (306 / 9) * i);
        down();
        color(!(i % 2)? "yellow": "red");
        begin_fill();
        for (int j = 0; j < 4; j++)
        {
            fd(400 - (94 + (2 * 306 / 9) * i) * (j % 2));
            left(90);
        } 
        end_fill();
    } 

    show();
    return 0;
}