build: turtle.c
	gcc -shared -I. turtle.c -lgdi32 -Wall -Wextra -Werror -o libturtle.dll

upload:
	giter . "..."