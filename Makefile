# Compiler and tools
CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -Werror -std=c17 -c -g
LIBS = -lgdi32
DEFINES = -D_USE_MATH_DEFINES

# Directories
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib

# Source files and targets
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = $(LIB_DIR)/libturtle.lib

# Phony targets
.PHONY: all clean

# Default rule
all: $(TARGET)

# Build the static library
$(TARGET): $(OBJECTS)
	mkdir -p $(LIB_DIR)
	$(AR) rcs $@ $^

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEFINES) $(LIBS) $< -o $@ 

# remove all outputs
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)