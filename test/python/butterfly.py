from turtle import *
from math import cos, pi, factorial, sin
# import numpy as np

speed(0)
width(4)
color('red')

def exp(x):
	e = 1
	for i in range(1, 30):
		e += x ** i / factorial(i)
	return e

# teta = [i for i in np.linspace(0,2 * pi, 100)]
teta = [i * 2 * pi / 99 for i in range(100)]


def rd(i):
	r = 100 * (1 - cos(i) * sin(3 * i))
	return r

up()
goto(100, 0)
down()

for i in  teta[1:]:
	r = rd(i)
	z = r * exp(i * 1j)
	x = z.real
	y = z.imag
	goto(x, y)
  
goto(100, 0)

width(1)

col = ['blue', 'purple' , 'green']

n = 0
for i in teta:
	r = rd(i)
	z = r * exp(i * 1j)
	x = z.real
	y = z.imag
	color(col[n % 3])
	n += 1
	goto(x, y)
	goto(0, 0)

hideturtle()
done()