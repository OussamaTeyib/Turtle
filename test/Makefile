CC := gcc
CFLAGS := -g -std=c2x -Wall -Wextra -Werror
LIBS := -lturtle

SRC := $(arg)
INCLUDE_DIR := ../include
LIB_DIR := ../lib

build: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(LIBS)

clean:
	rm -f *.exe