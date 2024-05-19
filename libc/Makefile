CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

ROOT?=

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -Iinclude

ARCHDIR=arch/i786

LIBC=\
stdio/printf.o \
string/strlen.o \

LIBC_OBJ=$(LIBC:.o=.libc.o)

ARCHIVE=libc.a

.PHONY: all clean install
.SUFFIXES: .o .libc.o .c .S

all: libc.a

libc.a: $(LIBC)
	$(AR) rcs $@ $(LIBC)

.c.o:
	$(CC) -MD -o $@ -c $< -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.c.S:
	$(CC) -MD -o $@ -c $< $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(ARCHIVE) *.a
	rm -f $(LIBC) *.o */*.o */*/*.o
	rm -f $(LIBC:.o=.d) *.d */*.d */*/*.d

install:
	mkdir -p $(ROOT)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
	mkdir -p $(ROOT)$(LIBDIR)
	cp $(ARCHIVE) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)

