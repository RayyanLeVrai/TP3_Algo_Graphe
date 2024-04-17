all: test_graphe

test_graphe: test_graphe.o graphe.o io_graphe.o pile.o file.o fap.o
	gcc -o test_graphe test_graphe.o graphe.o io_graphe.o pile.o file.o fap.o

graphe.o: graphe.c graphe.h pile.o file.o
	gcc -Wall -c graphe.c

io_graphe.o: io_graphe.c graphe.h
	gcc -Wall -c io_graphe.c

pile.o: pile.c graphe.h
	gcc -Wall -c pile.c

file.o: file.c graphe.h
	gcc -Wall -c file.c

fap.o: fap.c graphe.h
	gcc -Wall -c fap.c

test_graphe.o: test_graphe.c graphe.h pile.h file.h fap.h
	gcc -Wall -c test_graphe.c

clean:
	rm -f *.o test_graphe  *~ data/*~