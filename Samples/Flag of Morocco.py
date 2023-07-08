from turtle import *

up()
goto (-150, -150)
down()
color ('red')
begin_fill()
for i in range (4):
	forward(300-100*(i%2))
	left(90)
end_fill()

up()
goto (-40, -110)
down()
color ('green')
width(7)
left(36)
for i in range (5):
	forward(130)
	left(180-36)

hideturtle()	
mainloop()