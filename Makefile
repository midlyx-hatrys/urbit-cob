PREFIX := /usr/local
OPTFLAGS := -Og

CC := gcc
BASE_CFLAGS := -Wall -Wextra -Wpedantic -Wformat=2 -Wno-unused-parameter \
       -Wshadow -Wstrict-prototypes -Wold-style-definition -Wredundant-decls \
       -Wnested-externs -Wmissing-include-dirs -g
CFLAGS := $(BASE_CFLAGS) $(OPTFLAGS)
AR := ar
LDLIBS := -lgmp -lmmh3

.PHONY: all test clean install

all: liburbit-ob.a tests

clean:
	rm *.o *.a

test: tests
	./tests

install:
	mkdir -p $(PREFIX)/include $(PREFIX)/lib
	install -m 0644 urbit-ob.h $(PREFIX)/include/urbit-ob.h
	install -m 0644 liburbit-ob.a $(PREFIX)/lib/liburbit-ob.a

liburbit-ob.a: ob.o co.o
	$(AR) r $@ $?

tests: tests.o liburbit-ob.a
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
