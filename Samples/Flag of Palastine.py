from turtle import *

up()
goto (-150, -75)
down()
colors = ('black', 'white', 'green')
for i in range (3):
	color (colors[i%3])
	begin_fill()
	for j in range (4):
		forward (300-75*(j%2)*(i+1))
		left(90)
	end_fill()

goto (-150, -75)
left(45)
color ('red')
begin_fill()
for i in range (2):
	forward (160)
	left(90)
end_fill()

hideturtle()
mainloop()