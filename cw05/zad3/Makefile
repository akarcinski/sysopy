CC=gcc
CFLAG=-Wall

all:
	make integral
	make rec

integral:
	$(CC) $(CFLAG) main.c -o main


rec:
	$(CC) $(CFLAG) util.c -o util

report:
	make all
	./main 0.0001 1000 > report.txt
	./main 0.001 100 >> report.txt
	./main 0.01 100 >> report.txt
	./main 0.1 10 >> report.txt

clean:
	rm -f main util