build: turtle.c
	gcc -shared -I. turtle.c -g -o libturtle.dll -lgdi32 -Wall -Wextra -Werror

clean:
	rm -f *.exe