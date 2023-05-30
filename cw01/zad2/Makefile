CC=gcc
CFLAGS=-Wall
LIB=collecting_data
CLI = cli.c


c_lib_static: $(LIB).c $(CLI)
	make lib_static
	$(CC) $(CFLAGS) $(CLI) -o exec -L. $(LIB).a

run_lib_static: $(LIB).c $(CLI)
	make c_lib_static
	./exec

c_lib_shared: $(LIB).c $(CLI)
	export LD_LIBRARY_PATH=.
	make lib_shared
	$(CC) $(CFLAGS) $(CLI) -o exec -L. $(LIB).so

run_lib_shared: $(LIB).c $(CLI)
	make c_lib_shared
	./exec

run_lib_dll: $(LIB).c $(CLI)
	make c_lib_shared
	$(CC) $(CFLAGS) $(CLI) -o exec -ldl -D DLL
	./exec

all:
	make lib_static lib_shared

lib_static: $(LIB).c
	$(CC) $(CFLAGS) -c $(LIB).c
	ar cr $(LIB).a $(LIB).o

lib_shared: $(LIB).c
	$(CC) $(CFLAGS) -c -fPIC $(LIB).c
	$(CC) $(CFLAGS) -shared -o $(LIB).so $(LIB).o

clean:
	rm -rf *.o *.a *.so exec

