PREFIX ?= /usr/local/bin

CFLAGS += -std=c99 -march=native -O3 -pipe
CFLAGS += -Wall
CFLAGS += -Wconversion
CFLAGS += -Wdouble-promotion
CFLAGS += -Wextra
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wold-style-definition
CFLAGS += -Wpedantic
CFLAGS += -Wshadow

all: xhidecursor

xhidecursor: main.c Makefile
	$(CC) $(CFLAGS) -o $@ $< -lX11 -lXfixes -lXi

install: all
	install -D xhidecursor $(DESTDIR)$(PREFIX)/xhidecursor

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/xhidecursor

clean:
	rm -f xhidecursor

.PHONY: all install uninstall clean
