from turtle import *

up()
goto (-150, -100)
down()
color ('red')
for i in range (4):
	forward (300-100*(i%2))
	left (90)

begin_fill()
forward (100)
for i in range (3):
	forward (200)
	left(90)

left (60)
for i in range (5):
	for j in range (2):
		forward (40)
		right (120)
	right(120)

end_fill()

hideturtle()	
done()