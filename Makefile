mymemory: main.o datatype.o list.o bst.o memory.o
	cc -o mymemory main.o datatype.o list.o bst.o memory.o
main.o: main.c list.h
	cc -c main.c
datatype.o: datatype.c datatype.h
	cc -c datatype.c
list.o: list.c list.h memory.h
	cc -c list.c
bst.o: bst.c bst.h
	cc -c bst.c
memory.o: memory.c memory.h
	cc -c memory.c
clean:
	rm -f run *.o core