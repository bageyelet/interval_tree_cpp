COMPILER=g++
FLAGS=-g

all: Interval IntervalTree PiTree main
	${COMPILER} Interval.o IntervalTree.o PiTree.o main_pitree.o ${FLAGS}

Interval:
	${COMPILER} -c Interval.cpp ${FLAGS}

IntervalTree:
	${COMPILER} -c IntervalTree.cpp ${FLAGS}

PiTree:
	${COMPILER} -c PiTree.cpp ${FLAGS}

main:
	${COMPILER} -c main_pitree.cpp ${FLAGS}

clean:
	rm *.o a.out