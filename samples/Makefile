CC = gcc
CFLAGS = -g -std=c2x -Wall -Wextra -Werror
LIBS = -lturtle
DEFINES = -D_USE_MATH_DEFINES

SRCS = $(wildcard *.c)
BINS = $(patsubst %.c, %.exe, $(SRCS))

INCLUDE_DIR := ../include
LIB_DIR := ../lib

all: $(BINS)

%.exe: %.c
	$(CC) $(CFLAGS) $(DEFINES) $< -o $@ -I$(INCLUDE_DIR) -L$(LIB_DIR) $(LIBS)

.PHONY: clean build

build:
	$(CC) $(CFLAGS) $(DEFINES) $(src) -o $(patsubst %.c, %.exe, $(src)) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(LIBS)

clean:
	rm -f *.exe