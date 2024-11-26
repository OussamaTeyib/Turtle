# Compiler and tools
CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -Werror -std=c17 -g
LIBS = -lturtle
DEFINES = -D_USE_MATH_DEFINES

# Directories
SRC_DIR = .
INCLUDE_DIR = ../src
BIN_DIR = .
LIB_DIR = ../lib

# Source files and targets
SOURCES = $(wildcard $(SRC_DIR)/*.c)
BINS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.exe, $(SOURCES))

# Phony targets
.PHONY: all clean

# Default rule
all: $(BINS)

# Compile source files into executables
$(BIN_DIR)/%.exe: $(SRC_DIR)/%.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(DEFINES) -I$(INCLUDE_DIR) -L$(LIB_DIR) $(LIBS) $< -o $@

# remove all outputs
clean:
	rm -rf $(BIN_DIR)/*.exe