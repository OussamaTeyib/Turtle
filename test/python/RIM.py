from turtle import *

up()
goto (-150, -50)
down()
color ('red')
begin_fill()
for i in range (4):
	forward (300-100*(i%2))
	left(90)
end_fill()

goto (-150, -20)
color ('green')
begin_fill()
for i in range(3):
	forward (300-160*(i%2))
	left(90)
end_fill()

goto (-50, 45)
color ('yellow')
begin_fill()
circle (50, 180)
end_fill()

right(180);

goto (-50, 60)
begin_fill()
color ('green')
circle (50, 180)
end_fill()

setheading(0);
goto (15, 40)
left(108)
color ('yellow')
begin_fill()
for i in range(5):
	forward (60)
	left(144)
end_fill()

hideturtle()
mainloop()