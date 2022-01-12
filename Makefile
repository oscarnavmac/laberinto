all: juego_laberinto

juego_laberinto: juego_laberinto.o laberinto.o IA_laberinto.o jugador_laberinto.o
	gcc -g -Wall -o juego_laberinto juego_laberinto.o laberinto.o IA_laberinto.o jugador_laberinto.o -lncurses

juego_laberinto.o: juego_laberinto.c include/laberinto.h include/IA_laberinto.h include/jugador_laberinto.h
	gcc -g -Wall -I ./include -c juego_laberinto.c

laberinto.o: laberinto.c include/laberinto.h
	gcc -g -Wall -I ./include -c laberinto.c 

IA_laberinto.o: IA_laberinto.c include/IA_laberinto.h
	gcc -g -Wall -I ./include -c IA_laberinto.c

jugador_laberinto.o: jugador_laberinto.c include/jugador_laberinto.h
	gcc -g -Wall -I ./include -c jugador_laberinto.c 

clean:
	rm *.o juego_laberinto
