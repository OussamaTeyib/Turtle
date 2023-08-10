CC := gcc
# 'gnu2x' defines 'M_PI' unlike the standard 'c2x'
CFLAGS := -g -std=gnu2x -Wall -Wextra -Werror
LDFLAGS := -shared
LIBS := -lgdi32
INCLUDE_DIR := include
SRC_DIR := src
LIB_DIR := lib

SRC := $(SRC_DIR)/turtle.c
TARGET := $(LIB_DIR)/libturtle.dll

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC) -I$(INCLUDE_DIR) $(LIBS)