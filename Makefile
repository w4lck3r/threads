CC      = gcc

CFLAGS  = -Wall -Wextra
CFLAGS += -g

LDFLAGS = -pthread

TOUS := aleazard compteur-gc rdv rdv3 rdvN rdvN_2sem

tout: ${TOUS}

aleazard: aleazard.c
	${CC} ${CFLAGS} $< -o $@ ${LDFLAGS}

compteur-gc: compteur-gc.c
	${CC} ${CFLAGS} $< -o $@ ${LDFLAGS}

rdv: rdv.c
	${CC} ${CFLAGS} $< -o $@ ${LDFLAGS}

rdv3: rdv3.c
	${CC} ${CFLAGS} $< -o $@ ${LDFLAGS}

rdvN: rdvN.c
	${CC} ${CFLAGS} $< -o $@ ${LDFLAGS}

rdvN_2sem: rdvN_2sem.c
	${CC} ${CFLAGS} $< -o $@ ${LDFLAGS}

compteur: tout
	./aleazard 1000 > genome.txt
	./compteur-gc -v genome.txt 8
	rm -f genome.txt

dat-compteur: tout
	./test.sh

gnuplot-compteur: dat-compteur
	./run.gp

clean:
	rm -f core *.o

realclean: clean
	rm -f ${TOUS}

.PHONY: all clean realclean
