PREFIX  ?= /usr/local
BINDIR  ?= $(PREFIX)/bin
MANDIR  ?= $(PREFIX)/share/man/man1

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
	install -D xhidecursor $(DESTDIR)$(BINDIR)/xhidecursor
	install -D -m 644 xhidecursor.1 $(DESTDIR)$(MANDIR)/xhidecursor.1

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/xhidecursor
	rm -f $(DESTDIR)$(MANDIR)/xhidecursor.1

clean:
	rm -f xhidecursor

.PHONY: all install uninstall clean

