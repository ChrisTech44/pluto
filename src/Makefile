CC=gcc
LDFLAGS = -L../lib
LDLIBS = -lelf -lbele
CFLAGS = -flto=auto -Werror -Wall -g -O0 -I../include
HEADERS = ../include/*
OBJ = pluto.o symbols.o list.o
TARGET = pluto

%.o : %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(OBJ) $(TARGET)
