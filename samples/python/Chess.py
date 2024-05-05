from turtle import *

speed(0)
	
for i in range(8):
	up()
	goto(-200, -200 + 50 * i)
	down()
	for j in range(8):
		if not (i + j) % 2:
			color('black')
		else:
			color('brown')

		# begin_fill()
		for k in range(4):
			forward(50)
			left(90)

		# end_fill()
		forward(50)

hideturtle()
mainloop()