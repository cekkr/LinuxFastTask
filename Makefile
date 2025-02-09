# Makefile semplice

CC = gcc
CFLAGS = -Wall -Wextra -g -I/opt/X11/include
LDFLAGS = -L/opt/X11/lib -lX11

SRC = main.c monitor.c decision.c control.c
OBJ = $(SRC:.c=.o)
TARGET = process_manager

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
