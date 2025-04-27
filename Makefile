TARGET = calc
SOURCE = calc.c config.h
CC = gcc
PREFIX = /usr/local

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) -o $@ $(SOURCE)

clean:
	rm -rf $(TARGET)
install:
	cp -f $(TARGET) $(PREFIX)/bin/$(TARGET)
	chmod 755 $(PREFIX)/bin/$(TARGET)
uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)
