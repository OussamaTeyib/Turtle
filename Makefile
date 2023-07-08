build: turtle.c
	gcc -shared -o libturtle.dll turtle.c -lgdi32 -Wall -Wextra -Werror

upload:
	giter . "..."