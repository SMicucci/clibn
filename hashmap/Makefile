#
# MODIFY HERE
#

LIB_n = hashmap
TEST = test
BIN_n = test.out
# directories names
DEPS_d = 
INC_d = include
SRC_d = src
OBJ_d = obj
BIN_d = bin

#
# LEAVE AS IT IS
#
CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2
DBGFLAGS = -g -O0
CPPFLAG = -I$(INC_d)
CPPFLAG += $(foreach dep,$(DEPS_d),-I../$(dep)/$(INC_d))

ifeq ($(strip $(DEPS_d)),)
	LBFLAG = 
else
	LBFLAG = -L$(realpath ../libs) $(foreach lib, $(DEPS_d), -l$(lib))
endif

SRC = $(filter-out $(SRC_d)/$(TEST).c, $(wildcard $(SRC_d)/*.c))
OBJ = $(patsubst $(SRC_d)/%, $(OBJ_d)/%, $(SRC:.c=.o))

.PHONY: dirs deps build debug clean port test

build: dirs $(BIN_d)/lib$(LIB_n).so port

debug: CFLAGS += $(DBGFLAGS)
debug: dirs $(BIN_d)/$(BIN_n)

dirs:
	@mkdir -p $(BIN_d) $(OBJ_d) ../libs ../include

deps:
	@if [ -n "$(strip $(DEPS_d))" ]; then \
		for dep in $(DEPS_d); do \
			$(MAKE) -C ../$$dep build || exit 1; \
		 done; \
	 fi

port: $(BIN_d)/lib$(LIB_n).so
	@cp $(BIN_d)/lib$(LIB_n).so ../libs
	@cp $(INC_d)/*.h ../include

$(OBJ): $(SRC) deps
	$(CC) -c -fPIC $(CPPFLAG) -o $@ $< $(CFLAGS)

$(BIN_d)/lib$(LIB_n).so: $(OBJ)
	$(CC) -shared -o $@ $^ $(LBFLAG)

$(BIN_d)/$(BIN_n): src/$(TEST).c $(BIN_d)/lib$(LIB_n).so
	$(CC) -L$(BIN_d) $(LBFLAG) -Wl,-rpath,$(realpath $(BIN_d)) -o $@ $< -l$(LIB_n) $(CFLAGS)

test: debug
	@valgrind -s $(BIN_d)/$(BIN_n)

clean:
	@rm -rf $(BIN_d) $(OBJ_d)
	@rm -f ../include/$(LIB_n).h ../libs/lib$(LIB_n).so
