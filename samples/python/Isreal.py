from turtle import *

bgcolor('grey')
up()
goto(-150, -100)
down()
color ('white')
begin_fill()
for i in range (4):
	forward(300-100*(i%2))
	left(90)
end_fill()

up()
goto(-150, -80)
down()
color ('blue')
begin_fill()
for i in range(4):
	forward (300-140*(i%2))
	left(90)
end_fill()

up()
goto(-150, -50)
down()
color ('white')
begin_fill()
for i in range (4):
	forward(300-200*(i%2))
	left(90)
end_fill()

up()
goto(-30, -15)
down()
width(5)
color ('blue')
for i in range (3):
	forward(60)
	left(120)

up()
goto(-30, 20)
down()
width(5)
color ('blue')
for i in range (3):
	forward(60)
	right(120)

hideturtle()
mainloop()