CC = gcc
CFLAGS = -g -std=c2x -Wall -Wextra -Werror
LDFLAGS = -shared
LIBS = -lgdi32
DEFINES = -D_USE_MATH_DEFINES

INCLUDE_DIR = include
SRC_DIR = src
LIB_DIR = lib
RELEASE_DIR = release

TARGET = $(LIB_DIR)\libturtle.dll
SRC = $(SRC_DIR)\turtle.c

all: $(TARGET) clean

# build the library
$(TARGET): $(SRC)
	@mkdir -p $(LIB_DIR)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(DEFINES) -o $@ $(SRC) -I$(INCLUDE_DIR) $(LIBS)

.PHONY: clean push zip release

# zip release's files
zip:
	@mkdir -p $(RELEASE_DIR)
	@7z a -tzip $(RELEASE_DIR)/$(ver).zip $(INCLUDE_DIR)/* $(LIB_DIR)/*

# publish a release
release:
	@git tag -a $(ver) -m "Release $(ver)"
	@git push origin $(ver)
	@gh release create $(ver)
	@gh release upload $(ver) $(RELEASE_DIR)/$(ver).zip

# push to Github
push:
	@git add .
	@git commit -m "$(msg)"
	@git push origin main

# remove all executables
clean:
	@rm -f samples/*.exe