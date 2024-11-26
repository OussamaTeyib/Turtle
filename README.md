## C Turtle Graphics
This is a C implementation of Python's Turtle Graphics using Windows API.

### Overview:
Turtle Graphics is a popular educational tool that introduces programming concepts through drawing. This implementation allows you to create graphics and drawings using a turtle-like interface in C.

<!--
### Getting Started:
- Download the latest release from [Releases page](https://github.com/OussamaTeyib/Turtle/releases).
-->

### Usage:
run:
- `gcc -o main main.c -D_USE_MATH_DEFINES -I<your-include-path> -L<your-lib-path> -lturtle`
- `main.exe`

**Exemple**:
> Many samples can be found [here](samples).
<table>
<tr>
    <td><b>C</b></td>
    <td><b>Python</b></td>
</tr>
<td>
  <pre lang = "c">
#include &lt;turtle.h&gt;

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
                forward(400 - (94 + (2 * 306 / 9) * i) * (j % 2));
                left(90);
            } 
            end_fill();
        } 

        show();
        return 0;
    }
</pre>
</td>
<td>
  <pre lang="python">
from turtle import *

    for i in range(5):
    	up()
    	goto(-200, -153 + (306 / 9) * i)
    	down()
    	color('yellow' if not i % 2 else 'red')
    	begin_fill()

    	for j in range(4):
    		forward(400 - (94 + (2 * 306 / 9) * i) * (j % 2))
    		left(90)

    	end_fill()

    done()
</pre>
</td>
</table>

### License:
This project is licensed under the [MIT License](LICENSE).
