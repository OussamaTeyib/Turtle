CC := gcc
CFLAGS := -g -std=c2x -Wall -Wextra -Werror
LDFLAGS := -shared
LIBS := -lgdi32
DEFINES := -D_USE_MATH_DEFINES

INCLUDE_DIR := include
SRC_DIR := src
LIB_DIR := lib

SRC := $(SRC_DIR)\turtle.c
TARGET := $(LIB_DIR)\libturtle.dll

all: $(TARGET) clear

$(TARGET): $(SRC)
	@mkdir -p lib
	@$(CC) $(CFLAGS) $(LDFLAGS) $(DEFINES) -o $@ $(SRC) -I$(INCLUDE_DIR) $(LIBS)

.PHONY: clear push

push:
	@git add .
	@git commit -m $(msg)
	@git push orgin main

clear:
	@rm -r samples/*.exe