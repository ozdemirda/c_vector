CC = gcc

SOURCE_DIR = src
INCLUDE_DIR = include
OBJECT_DIR = obj

_create_object_dir := $(shell mkdir -p $(OBJECT_DIR))

CFLAGS = -I$(INCLUDE_DIR) -c -fPIC -fstack-protector-all \
	-Wstrict-overflow -Wformat=2 -Wformat-security -Wall -Wextra \
	-g3 -O3 -Werror
LFLAGS = -shared -lpthread

SOURCE_FILES = $(SOURCE_DIR)/cvector.c
HEADER_FILES = $(INCLUDE_DIR)/cvector.h
OBJ_FILES = $(SOURCE_FILES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)

default: all

all: libcvector.so

libcvector.so: $(OBJ_FILES)
	$(CC) -o libcvector.so $(OBJ_FILES) $(LFLAGS)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADER_FILES)
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -rf libcvector.so $(OBJECT_DIR)
