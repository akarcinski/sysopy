CC=gcc
CFLAG=-Wall
LIB=memhandling.c quehandling.c semhandling.c
all:
	$(CC) $(CFLAG) client.c $(LIB) -o client
	$(CC) $(CFLAG) hairdresser.c $(LIB) -o hairdresser
	$(CC) $(CFLAG) simulationengine.c $(LIB) -o engine
clean:
	rm -f client hairdresser engine
