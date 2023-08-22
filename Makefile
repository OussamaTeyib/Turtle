CC := gcc
CFLAGS := -g -std=c2x -Wall -Wextra -Werror
LDFLAGS := -shared
LIBS := -lgdi32
DEFINES := -D_USE_MATH_DEFINES

INCLUDE_DIR := include
SRC_DIR := src
LIB_DIR := lib

SRC := $(SRC_DIR)/turtle.c
# INCLUDES: $(INCLUDE_DIR)/turtle.h
TARGET := $(LIB_DIR)/libturtle.dll

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DEFINES) -o $(TARGET) $(SRC) -I$(INCLUDE_DIR) $(LIBS)