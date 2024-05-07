CC := gcc
CFLAGS := -g -std=c2x -Wall -Wextra -Werror
LIBS := -lturtle
DEFINES := -D_USE_MATH_DEFINES

SRC := $(src)
INCLUDE_DIR := ../include
LIB_DIR := ../lib


$($(src):.c, .exe): $(SRC)
	$(CC) $(CFLAGS) $(DEFINES) $(SRC) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(LIBS)

.PHONY: clean

clean:
	rm -f *.exe