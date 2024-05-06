CXX:=gcc
CFLAGS:=-g -Wall

INCDIR:=include
LOGDIR:=log

CFILES:=lib
OFILES:=obj

all: $(OFILES)/ataksh

$(OFILES)/ataksh: $(CFILES)/ataksh.c $(CFILES)/logger.c
	$(CXX) $(CFLAGS) -I$(INCDIR) $^ -o $@

run: all
	$(OFILES)/ataksh

.PHONY: clean

clean:
	rm -f $(OFILES)/* $(LOGDIR)/ataksh.log

