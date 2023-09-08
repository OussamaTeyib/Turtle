from turtle import *

up()
goto(-150, -100)
down()
color('red')
begin_fill()
for i in range(4):
	forward(300 - 100 * (i % 2))
	left(90)
end_fill()

up()
goto(-150, -70)
down()
color('green')
begin_fill()
for i in range(4):
	forward(300 - 160 * (i % 2))
	left(90)
end_fill()

up()
goto(0, -60)
down()
color('yellow')
begin_fill()
circle(50)
end_fill()

up()
goto(0, -45)
down()
color('green')
begin_fill()
circle(50)
end_fill()

up()
goto(18, -5)
down()
color('yellow')
left(108)
begin_fill()
for i in range(5):
	forward(60)
	left(144)
end_fill()

hideturtle()
done()