CC = gcc
CFLAGS ?= -g -Wall
LIBS = -pthread
SRCS = $(shell find src/ -iname \*.c -printf "%f\n")
OBJ = $(addprefix obj/, $(SRCS:.c=.o))
BIN = main

VPATH = src/

all: $(BIN)
	echo $(OBJ)

-include obj/*.d

obj/%.o: %.c
	@mkdir -p obj
	$(CC) -c -o $@ $< -MMD $(CFLAGS) $(LIBS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -rf $(BIN) obj/
