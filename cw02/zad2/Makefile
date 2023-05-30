CC=gcc
CFLAG=-Wall

report: reverse.c
	$(CC) $(CFLAG) reverse.c -o exec -D SIZE=1
	cat /dev/random | head -10 > input.txt

	echo "N_CHAR = 1" > pomiar_zad_2.txt
	./exec input.txt out.txt >> pomiar_zad_2.txt
	./exec reverse.c out.txt >> pomiar_zad_2.txt
	./exec Makefile out.txt >> pomiar_zad_2.txt

	$(CC) $(CFLAG) reverse.c -o exec -D SIZE=1024
	echo "N_CHAR = 1024" >> pomiar_zad_2.txt
	./exec input.txt out.txt >> pomiar_zad_2.txt
	./exec reverse.c out.txt >> pomiar_zad_2.txt
	./exec Makefile out.txt >> pomiar_zad_2.txt
	rm -f out.txt input.txt exec

clean:
	rm -rf *.a *.o *.txt exec
