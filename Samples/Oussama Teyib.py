from turtle import *

hideturtle()
pu()
goto (-280, 0)
colors = ('red', 'brown', 'purple', 'green', 'black', 'orange', 'blue')
name = 'OUSSAMA'
for i in range (7):
	color(colors[i])
	write(name[i], font = ('times new roman', 20, 'italic'))
	forward (80)

done()