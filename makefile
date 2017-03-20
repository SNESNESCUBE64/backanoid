INCPATH=./include
OBJPATH=./obj
CC=cc
SRC=./src

Backanoid: main.o scoreboard.o set_ticker.o arrow.o
	${CC} -o Backanoid scoreboard.o main.o set_ticker.o -lcurses
	mv scoreboard.o main.o set_ticker.o arrow.o ${OBJPATH}

main.o: ${SRC}/main.c ${INCPATH}/scoreboard.h
	${CC} -I${INCPATH} -c ${SRC}/main.c -lcurses

scoreboard.o: ${SRC}/scoreboard.c ${INCPATH}/scoreboard.h
	${CC} -I${INCPATH} -c ${SRC}/scoreboard.c -lcurses


set_ticker.o: ${SRC}/set_ticker.c ${INCPATH}/set_ticker.h
	${CC} -I${INCPATH} -c ${SRC}/set_ticker.c

arrow.o: ${SRC}/arrow.c
	${CC} -c ${SRC}/arrow.c -lcurses

clean:
	-rm -f Backanoid
	-rm -f $(OBJPATH)/*.o
	-rm -f *.o
