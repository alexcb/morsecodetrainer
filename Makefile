CC=gcc
CCFLAGS=-O3 -std=gnu11 -Wall -Werror -I./src
LDFLAGS=-lmpg123 -lmicrohttpd -lpthread -lm -lpulse -lpulse-simple

SRC=$(wildcard src/**/*.c src/*.c)
OBJ=$(SRC:%.c=%.o)

TESTSRC=$(wildcard tests/**/*.c tests/*.c)
TESTOBJ=$(TESTSRC:%.c=%.o)

OBJWITHOUTMAIN := $(filter-out src/main.o,$(OBJ))

build: my123 test

my123: $(OBJ)
	$(CC) $(CCFLAGS) -o my123 $^ $(LDFLAGS)

test: $(OBJWITHOUTMAIN) $(TESTOBJ)
	$(CC) $(CCFLAGS) -o test $^ $(LDFLAGS)

# To obtain object files
%.o: %.c
	$(CC) -c $(CCFLAGS) $< -o $@

clean:
	rm -f my123 test $(OBJ) $(TESTOBJ)
