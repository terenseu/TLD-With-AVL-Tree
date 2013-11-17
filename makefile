COMPILER = gcc
FLAGS = -W -Wall
EXEC = ./assignment 01/01/2000 01/01/2004 small.txt

assignment: tldmonitor.o date.o tldlist.o
	${COMPILER} ${FLAGS} -o assignment tldmonitor.o date.o tldlist.o

%.o: %.c
	${COMPILER} ${FLAGS} -c $*.c

clean:
	rm -f *.o *~ assignment

try:
	${EXEC}

test:
	valgrind --leak-check=yes ${EXEC}
