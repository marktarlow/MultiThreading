CC = gcc
HEADER = multiThreaderSorter_thread.h

default: program

multiThreaderSorter_thread.o: multiThreaderSorter_thread.c $(HEADER)
	$(CC) -c  multiThreaderSorter_thread.c -o multiThreaderSorter_thread.o

mergesort.o: mergesort.c
	$(CC) -c mergesort.c -o mergesort.o

program: multiThreaderSorter_thread.o mergesort.o
	$(CC) -lpthread  multiThreaderSorter_thread.o mergesort.o

clean:
	-rm -f multiThreaderSorter_thread.o
	-rm -f mergesort.o
	-rm -f program

