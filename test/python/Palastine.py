from turtle import *

up()
goto(-120, -90)
down()
colors = ('black', 'white', 'green')
for i in range(3):
	color(colors[i % 3])
	begin_fill()
	for j in range(4):
		forward(240 - 60 * (j % 2) * (i + 1))
		left(90)
	end_fill()

goto(-120, -90)
left(45)
color('red')
begin_fill()
for i in range(2):
	forward(128)
	left(90)
end_fill()

hideturtle();
done()