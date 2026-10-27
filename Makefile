CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11
TARGET = lexer
SOURCES = main.c lexer.c

all: $(TARGET)

$(TARGET): $(SOURCES) lexer.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all clean
