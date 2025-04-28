TARGET = calc
SOURCE = calc.c config.h
PREFIX = /usr/local

MANPAGE = calc.1
MANPREFIX = $(PREFIX)/share/man
CC = gcc

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) -o $@ $(SOURCE) -lm

clean:
	rm -rf $(TARGET)
install:
	cp -f $(TARGET) $(PREFIX)/bin/$(TARGET)
	chmod 755 $(PREFIX)/bin/$(TARGET)

	mkdir -p $(MANPREFIX)/man1
	cp $(MANPAGE) $(MANPREFIX)/man1/$(MANPAGE)
	chmod 644 $(MANPREFIX)/man1/$(MANPAGE)

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)
	rm -f $(MANPREFIX)/man1/$(MANPAGE)
