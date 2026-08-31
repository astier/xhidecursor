PREFIX     ?= /usr/local
BINDIR     ?= $(PREFIX)/bin
MANDIR     ?= $(PREFIX)/share/man/man1
PKG_CONFIG ?= pkg-config

PKGS = x11 xfixes xi

CFLAGS += -std=c99
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
	$(CC) $(CPPFLAGS) $(CFLAGS) `$(PKG_CONFIG) --cflags $(PKGS)` \
	    -o $@ main.c $(LDFLAGS) `$(PKG_CONFIG) --libs $(PKGS)` $(LDLIBS)

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(MANDIR)
	install -m 755 xhidecursor $(DESTDIR)$(BINDIR)/xhidecursor
	install -m 644 xhidecursor.1 $(DESTDIR)$(MANDIR)/xhidecursor.1

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/xhidecursor
	rm -f $(DESTDIR)$(MANDIR)/xhidecursor.1

clean:
	rm -f xhidecursor

.PHONY: all install uninstall clean
