#ifndef LABERINTO

#define LABERINTO

#include <stdio.h>
#include <string.h>
#include <ncurses.h>

typedef
struct 
{
	int x;
	int y;
} coordenadas; 	

typedef
struct 
{
	char **matriz;
	int columnas;
	int filas;
	coordenadas Inicio;
	coordenadas Fin;
}mapa;

void crea_matriz_laberinto(int nivel, mapa *laberinto);
void libera_matriz_laberinto();
//void dibuja_laberinto(jugador *apuntador);

#endif

