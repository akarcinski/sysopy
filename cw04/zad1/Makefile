CC=gcc
CFLAG=-Wall

all:
	make forked
	make execld

forked: main.c
	$(CC) $(CFLAG) main.c -o forked

execld: main_execl.c
	$(CC) $(CFLAG) main_execl.c -o execld

report:
	make all

	echo "Ignore" 2> fork_report.txt
	./forked ignore 2>> fork_report.txt

	echo "\nHandler" 2>> fork_report.txt
	./forked handler 2>> fork_report.txt

	echo "\nMask" 2>> fork_report.txt
	./forked mask 2>> fork_report.txt

	echo "\nPending" 2>> fork_report.txt
	./forked pending 2>> fork_report.txt


	echo "Ignore" 2> execl_report.txt
	./execld ignore 2>> execl_report.txt

	echo "\nMask" 2>> execl_report.txt
	./execld mask 2>> execl_report.txt

	echo "\nPending" 2>> execl_report.txt
	./execld pending 2>> execl_report.txt


clean:
	rm -f forked execld fork_report.txt execl_report.txt
