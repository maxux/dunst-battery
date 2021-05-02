EXEC = dunst-battery
PREFIX = /usr/local/
CFLAGS += -W -Wall -O2 -pipe -ansi -std=gnu99
LDFLAGS += -lrtinfo

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXEC)
