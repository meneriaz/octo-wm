# Change this to fit your system.
PREFIX = /usr

VERSION = 0.0.0

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

CFLAGS = -std=c99 -pedantic -Wall -Werror -Wno-unused-parameter
LDLIBS = -lxcb -lxcb-util

all: octo-wm

options:
	@echo "octo-wm build options:"
	@echo "CFLAGS       = $(CFLAGS)"
	@echo "LDFLAGS      = $(LDFLAGS)"
	@echo "LDLIBS       = $(LDLIBS)"
	@echo "CC           = $(CC)"

octo-wm: $(OBJ)
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^

clean:
	@echo "Cleaning..."
	@$(RM) $(OBJ) octo-wm octo-wm-$(VERSION).tar.gz

dist: clean
	@echo "Creating tarball..."
	@mkdir -p octo-wm-$(VERSION)
	@cp -R LICENSE Makefile README.md src/ octo-wm-$(VERSION)
	@tar -cf octo-wm-$(VERSION).tar octo-wm-$(VERSION)
	@gzip octo-wm-$(VERSION).tar
	@rm -rf octo-wm-$(VERSION)

install: all
	@echo "Installing binaries to $(DESTDIR)$(PREFIX)/bin..."
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@cp -f octo-wm $(DESTDIR)$(PREFIX)/bin
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/octo-wm

uninstall:
	@echo "Removing binaries from $(DESTDIR)$(PREFIX)/bin..."
	@$(RM) $(DESTDIR)$(PREFIX)/bin/octo-wm

.PHONY: all options clean dist install uninstall

