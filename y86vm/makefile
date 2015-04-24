CC=c99
EXEC=y86vm
OBJS=*.o
SOURCES=*.c

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

.c.o:
	$(CC) -c *.c

clean:
	rm -f $(OBJS) $(EXEC) a.out
