from turtle import *
from math import pi, cos, sin
from time import sleep

# turtle speed
speed(0)

# draw the red circle
up()
goto(0, -150)
down()
color('red')
begin_fill()
circle(150)
end_fill()

# draw the chronometer
up()
goto(0, 0)
down()
for i in range(60):
	width(5)
	color('black')
	goto(140 * cos(i * pi / 30), 140 * sin(i * pi /30))
	hideturtle()
	goto(0, 0)

# number of seconds
n = 7

# countdown
def watch():
	for i in range(n, -1, -1):
		width(5)
		color('white')
		goto(140 * cos(i * pi / 30), 140 * sin(i * pi / 30))
		hideturtle()
		sleep(1)
		if i:
			color('black')
		goto(0, 0)
	
# driver code	
watch()
# keep the code running
mainloop()