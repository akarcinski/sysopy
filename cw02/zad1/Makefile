CC=gcc
CFLAG=-Wall

lib: main_lib.c
	$(CC) $(CFLAG) main_lib.c -o exec_lib

run_lib:
	./exec_lib a b example_1.txt out_lib.txt

sys: main_sys.c
	$(CC) $(CFLAG) main_sys.c -o exec_sys

run_sys:
	./exec_sys a b example_1.txt out_sys.txt
	chmod 664 out_sys.txt

raport: main_sys.c main_lib.c
	make lib
	make sys
	cp main_lib.c example_1.txt
	cp main_sys.c example_2.txt
	cp Makefile example_3.txt
	echo "BIBLIOTECZNE" > pomiar_zad_1.txt
	./exec_lib m S example_1.txt out_lib.txt >> pomiar_zad_1.txt
	./exec_lib c A example_2.txt out_lib.txt >> pomiar_zad_1.txt
	./exec_lib e D example_3.txt out_lib.txt >> pomiar_zad_1.txt
	echo "SYSTEMOWE" > pomiar_zad_1.txt
	./exec_sys m S example_1.txt out_lib.txt >> pomiar_zad_1.txt
	./exec_sys c A example_2.txt out_lib.txt >> pomiar_zad_1.txt
	./exec_sys e D example_3.txt out_lib.txt >> pomiar_zad_1.txt
	rm -f out_*.txt


clean:
	rm -rf *.o *.a exec_lib exec_sys out_lib.txt out_sys.txt example_*.txt