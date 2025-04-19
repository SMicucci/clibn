#
# MODIFY HERE
#

LIB_n = mylib
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

SRC = $(filter-out $(SRC_d)/$(TEST).c, $(wildcard $(SRC_d)/*.c))
OBJ = $(patsubst $(SRC_d)/%, $(OBJ_d)/%, $(SRC:.c=.o))

.PHONY: dirs build debug static clean port

build: dirs $(BIN_d)/lib$(LIB_n).so port

debug: CFLAGS += $(DBGFLAGS)
debug: dirs $(BIN_d)/$(BIN_n)

static: dirs $(BIN_d)/lib$(LIB_n).a

dirs:
	@mkdir -p $(BIN_d) $(OBJ_d) ../libs ../include

port: $(BIN_d)/lib$(LIB_n).so
	@cp $(BIN_d)/lib$(LIB_n).so ../libs
	@cp $(INC_d)/*.h ../include

$(OBJ): $(SRC) $(DEPS)
	$(CC) -c -fPIC $(CPPFLAG) -o $@ $< $(CFLAGS)

$(BIN_d)/lib$(LIB_n).so: $(OBJ)
	@if [ -n "$(strip $(DEPS_d))" ]; then \
		for dep in $(DEPS_d); do \
			$(MAKE) -C ../$$dep build || exit 1; \
		 done; \
	 fi
	$(CC) -shared -o $@ $^

$(BIN_d)/lib$(LIB_n).a: $(OBJ)
	ar rcs $@ $^

$(BIN_d)/$(BIN_n): src/$(TEST).c $(BIN_d)/lib$(LIB_n).a
	$(CC) -L$(BIN_d) -Wl,-rpath,$(BIN_d) -o $@ $< -l$(LIB_n) $(CFLAGS)

clean:
	$(RM) -r $(BIN_d) $(OBJ_d)
	$(RM) -r ../include/$(LIB_n).h ../libs/lib$(LIB_n).so
