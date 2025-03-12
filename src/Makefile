CC=gcc
LDFLAGS = -L../lib
LDLIBS = -lelf -lbele
CFLAGS = -Werror -Wall -g -O0 -I../include
HEADERS = ../include/pluto.h ../include/instruction_formats.h
OBJ = pluto.o
TARGET = pluto

%.o : %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(OBJ) $(TARGET)
