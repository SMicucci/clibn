
CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2
DBGFLAGS = -g -O0

SRC = src/vector.c
OBJ = $(patsubst src/%, obj/%, $(SRC:.c=.o))
DEPS = src/vector.h

BIN = bin

.PHONY: directories default debug static test clean

default: directories $(BIN)/libvector.so

debug: CFLAGS += $(DBGFLAGS)
debug: directories $(BIN)/libvector.so $(BIN)/test

static: directories $(BIN)/libvector.a

test: directories $(BIN)/test

directories:
	mkdir -p $(BIN)
	mkdir -p obj

obj/%.o: src/%.c $(DEPS)
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)

$(BIN)/libvector.so: $(OBJ)
	$(CC) -shared -o $@ $^

$(BIN)/libvector.a: $(OBJ)
	ar rcs $@ $^

$(BIN)/test: src/test.c $(BIN)/libvector.a
	$(CC) -L$(BIN) -Wl,-rpath,$(BIN) -o $@ $< -lvector $(CFLAGS)

clean:
	rm -rf $(BIN)
	rm -rf obj
