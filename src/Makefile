CC = cc
CFLAGS = -std=c99 -Wall -O2 -pedantic -Wextra -D_GNU_SOURCE
CFLAGS += -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes \
	  -Wmissing-prototypes
CC_COMPILE = $(CC) $(CFLAGS) -c

OBJFILES := $(patsubst %.c,%.o,$(wildcard *.c))

PREFIX=/usr/local

all: tfm

tfm : $(OBJFILES)
	$(CC) -o tfm $(OBJFILES) -lncurses -pthread

%.o: %.c
	$(CC_COMPILE) -o $@ $<

clean:
	@echo cleaning directory
	@rm -f *.o tfm

install: all
	@echo installing tfm to ${PREFIX}/bin
	@mkdir -p ${PREFIX}/bin
	@cp -f tfm ${PREFIX}/bin
	@chmod 755 ${PREFIX}/bin/tfm
