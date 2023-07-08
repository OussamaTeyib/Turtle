from turtle import *
from math import pi, sin, cos

speed(5)
up()
goto(-150, -100)
down()
color ('blue')
begin_fill()
for i in range (4):
	forward (300-100*(i%2))
	left(90)
end_fill()

up()
goto(0, 0)
down()
color ('yellow')
left(36)
for i in range (12):
	up()
	goto (50*cos(i*pi/6), 50*sin(i*pi/6))
	down()
	begin_fill()
	for j in range (5):
		forward(15)
		left(144)
	end_fill()
	
hideturtle()
done()