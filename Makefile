CC=gcc
FLAGS=-Wall -g
BIN=xtrace
OBJS=$(shell ls *.c | sed -e "s/\.c/.o/g")

all: $(OBJS)
	$(CC) $(OBJS) $(FLAGS) $(LIBS) -o $(BIN)

%.o: %.c
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf *.o
	rm -rf $(BIN)
