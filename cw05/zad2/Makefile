CC=gcc
CFLAG=-Wall

all: main.c
	make compile
	make report
compile: main.c
	$(CC) $(CFLAG) main.c -o main
report:
	./main 0.0000001 1000 > report.txt
	./main 0.000001 1000 >> report.txt
	./main 0.00001 1000 >> report.txt
	./main 0.1 10 >> report.txt
clean:
	rm -f report.txt main