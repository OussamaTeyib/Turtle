CC := gcc
CFLAGS := -g -std=c2x -Wall -Wextra -Werror
LIBS := -lturtle
DEFINES := -D_USE_MATH_DEFINES

SRC := $(arg)
INCLUDE_DIR := ../include
LIB_DIR := ../lib

build: $(SRC)
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(LIBS)

clean:
	rm -f *.exe