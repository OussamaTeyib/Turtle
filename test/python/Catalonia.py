from turtle import *

for i in range(5):
    penup()
    goto(-200, -153 + (306 / 9) * i)
    pendown()
    color("yellow" if not (i % 2) else "red")
    begin_fill()

    for j in range(4):
        forward(400 - (94 + (2 * 306 / 9) * i) * (j % 2))
        left(90)

    end_fill()

done()