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

all: $(TARGET) clean

$(TARGET): $(SRC)
	@mkdir -p lib
	@$(CC) $(CFLAGS) $(LDFLAGS) $(DEFINES) -o $@ $(SRC) -I$(INCLUDE_DIR) $(LIBS)

.PHONY: clean commit push

f = .

commit:
	@git add $(f)
	@git commit -m $(msg)

push:
	@git push origin main

clean:
	@rm -r samples/*.exe