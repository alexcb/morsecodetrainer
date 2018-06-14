CC=gcc
CCFLAGS=-O3 -std=gnu11 -Wall -Werror -I./src
LDFLAGS=-lmpg123 -lmicrohttpd -lpthread -lm -lpulse -lpulse-simple -lcurses

SRC=$(wildcard src/**/*.c src/*.c)
OBJ=$(SRC:%.c=%.o)

TESTSRC=$(wildcard tests/**/*.c tests/*.c)
TESTOBJ=$(TESTSRC:%.c=%.o)

OBJWITHOUTMAIN := $(filter-out src/main.o,$(OBJ))

build: morse-code-trainer test

morse-code-trainer: $(OBJ)
	$(CC) $(CCFLAGS) -o morse-code-trainer $^ $(LDFLAGS)

test: $(OBJWITHOUTMAIN) $(TESTOBJ)
	$(CC) $(CCFLAGS) -o test $^ $(LDFLAGS)

# To obtain object files
%.o: %.c
	$(CC) -c $(CCFLAGS) $< -o $@

install: morse-code-trainer
	cp morse-code-trainer ~/bin/.

clean:
	rm -f morse-code-trainer test $(OBJ) $(TESTOBJ)
