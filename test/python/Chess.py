from turtle import *

speed(100)
	
for i in range (8):
	up()
	goto (-320, -320+80*i)
	down()
	for j in range (8):
		if not (i + j) % 2:
			color ('black')
		else:
			color ('brown')
		begin_fill()
		for k in range (4):
			forward (80)
			left(90)
		forward (80)
		end_fill()

hideturtle()
mainloop()