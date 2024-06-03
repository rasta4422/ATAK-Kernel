CC				:= gcc
CFLAGS			:= -g -Wall -I../arch/x86/boot/include/

all: test

test:
	$(CC) $(CFLAGS) ../arch/x86/boot/lenstr.c unit_tests.c -o $@.o
	./$@.o

clean:
	rm -rf *.o || True
