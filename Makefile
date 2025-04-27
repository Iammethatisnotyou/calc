TARGET = calc
SOURCE = calc.c config.h
BACKUP = bk_calc.c
CC = gcc

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) -o $@ $(SOURCE)

clean:
	rm -rf $(TARGET)
copy:
	cp calc.c $(BACKUP)
