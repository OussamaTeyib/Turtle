from turtle import *

up ()
goto(-150, 75)
down()
colors = ('black', 'white', 'green')
for i in range (3):
	color (colors[i])
	begin_fill()
	for j in range (4):
		forward (300 - 75 * (j%2)*(i+1))
		right(90)
	end_fill()

up()
goto(-105, -55)
down()
color ('red')
for i in range (3):
	left(36)
	begin_fill()
	for j in range (5):
		forward(40)
		left(144)
	end_fill()
	right(36)
	up()
	forward(90)
	down()

hideturtle()
done()