#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <laberinto.h>
#include <ncurses.h>

void crea_matriz_laberinto(int nivel, mapa *laberinto)
{
	FILE *apu;
	int columnas, filas, contador = 0, i, j;
	char direccion[27] = "./laberintos/laberinto";
	char caracter, *arreglo, *dimension, *token;
	char num[4][2] = { {"1"}, {"2"}, {"3"}, {"4"} };
	
	dimension = (char *)malloc(sizeof(char));
	arreglo = (char *)malloc(sizeof(char));

	strcat(direccion, *(num + --nivel));
	strcat(direccion, ".txt");

	apu = fopen(direccion, "rt");
	
	if (apu)
	{
		while ((caracter = fgetc(apu)) != EOF) // Para crear la cadena que contendrá la dimensión de nuestra matriz
		{
			if (caracter == '\n')
				break;
			dimension[contador] = caracter;
			contador++;
			dimension = realloc(dimension, (contador + 1)*sizeof(char));
		}
		dimension[contador] = '\0';
	
		contador = 0;
		while ((caracter = fgetc(apu)) != EOF) // Para crear la cadena que contendrá el contenido de nuestra matriz (el laberinto)
		{	
			if (caracter != '\n')
			{
				arreglo[contador] = caracter;
				contador++;
				arreglo = realloc(arreglo, (contador + 1)*sizeof(char));
			} 
		}
		arreglo[contador] = '\0';
	}
	
	else
		{
			printf("\nNo se encontró el archivo\n");
			exit(1);
		}

	token = strtok(dimension, " ");
	filas = atoi(token);
	token = strtok(NULL, "\n");
	columnas = atoi(token);
	
	free(dimension);
 
	laberinto->matriz = (char **)malloc(filas*sizeof(char *));
	
	for (i = 0; i < filas; i++)
	{
		*(laberinto->matriz + i) = malloc(columnas*sizeof(char));
	}
	
	laberinto->filas = filas;
	laberinto->columnas = columnas;
	
	contador = 0;
	for (i = 0; i < filas; i++)
	{
		for (j = 0; j < columnas; j++)
		{
			laberinto->matriz[i][j] = arreglo[contador]; 
			contador++;
			if (laberinto->matriz[i][j] == 'I')
			{
				laberinto->Inicio.x = j;
				laberinto->Inicio.y = i;
			}

				if (laberinto->matriz[i][j] == 'F')
			{
				laberinto->Fin.x = j;
				laberinto->Fin.y = i;
			}
		}
	}

	free(arreglo);
}

void libera_matriz_laberinto(mapa *laberinto)
{
	int i;

	for (i = 0; i < laberinto->filas; i++)
		free(*(laberinto->matriz + i));

	free(laberinto->matriz);
}


