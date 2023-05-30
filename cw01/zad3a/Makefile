CC=gcc
CFLAGS=-Wall
LIB=collecting_data
CLI = cli.c
MYDIR=.
NUMBERS = 0 1 2 3 4
all:
	make c_lib_static
	make c_lib_shared
	make c_lib_dll

make_report:
	make create_test
	make all OFLAG="-O0"
	make run_tests TMP="O0"
	make all OFLAG="-O1"
	make run_tests TMP="O1"
	make all OFLAG="-O2"
	make run_tests TMP="O2"
	make all OFLAG="-O3"
	make run_tests TMP="O3"
	make all OFLAG="-Os"
	make run_tests TMP="Os"


create_test: $(MYDIR)/*
	echo "init 1024" > test.in

	$(foreach file, $(wildcard *.[h,c]), echo "count "$(file) >> test.in;)
	$(foreach number, $(NUMBERS), echo "show "$(number) >> test.in;)
	$(foreach number, $(NUMBERS), echo "delete "$(number) >> test.in;)
	echo "destroy" >> test.in
	echo "exit" >> test.in
	#for i in $(ls -1 | wc -l)
#   for file in $^ ; do \
#  		echo "count" $${file} >> test.in ;\
#  	done

run_tests:
	make all
	sudo mv collecting_data.so /usr/lib
	./exec_static < test.in > resault_static_$(TMP).txt
	./exec_shared < test.in > resault_shared_$(TMP).txt
	./exec_dll < test.in > resault_dll_$(TMP).txt

c_lib_static: $(LIB).c $(CLI)
	make lib_static
	$(CC) $(CFLAGS) $(OFLAG) $(CLI) -o exec_static -L. $(LIB).a

run_lib_static: $(LIB).c $(CLI)
	make c_lib_static
	./exec_static

c_lib_shared: $(LIB).c $(CLI)
	export LD_LIBRARY_PATH=.
	make lib_shared
	$(CC) $(CFLAGS) $(OFLAG) $(CLI) -o exec_shared -L. $(LIB).so

run_lib_shared: $(LIB).c $(CLI)
	make c_lib_shared
	./exec_shared

c_lib_dll: $(LIB).c $(CLI)
	make c_lib_shared
	$(CC) $(CFLAGS) $(OFLAG) $(CLI) -o exec_dll -ldl -D DLL

run_lib_dll: $(LIB).c $(CLI)
	make c_lib_dll
	./exec_dll


lib_static: $(LIB).c
	$(CC) $(CFLAGS) $(OFLAG) -c $(LIB).c
	ar cr $(LIB).a $(LIB).o

lib_shared: $(LIB).c
	$(CC) $(CFLAGS) $(OFLAG) -c -fPIC $(LIB).c
	$(CC) $(CFLAGS) $(OFLAG) -shared -o $(LIB).so $(LIB).o

clean:
	rm -rf *.o *.a *.so exec_* *.txt test.in

