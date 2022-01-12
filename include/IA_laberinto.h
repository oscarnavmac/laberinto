#ifndef IA

#define IA

#include <stdio.h>
#include <laberinto.h>

typedef
	struct _nodo
	{
		coordenadas posicion;
		struct _nodo *arriba;
		struct _nodo *abajo;
		struct _nodo *izquierda;
		struct _nodo *derecha;
		struct _nodo *sig;
}nodo;

typedef
	struct
	{
		nodo *lista;
		int num_nodos;
}lista_adyacencia;

typedef
	struct
	{
		nodo *nodo;
		char color;
		nodo *paternidad;
		int distancia;
}tabla_busqueda;

typedef 
	struct
	{
    	int *arreglo;        
   	unsigned int N;  
    	unsigned int H;  
    	unsigned int T;  
}cola;
	
//FUNCIONES DEL GRAFO
int crea_lista_adyacencia(lista_adyacencia *grafo, mapa matriz);

nodo *crea_nodo(int y, int x);

int agrega_nodo(lista_adyacencia *grafo, int y, int x);

nodo *busca_nodo(lista_adyacencia grafo, int y, int x);

int modulo(int a, int n);

void libera_lista_adyacencia(lista_adyacencia *grafo);

//FUNCIONES DE BUSQUEDA;
int BFS(tabla_busqueda *vector, int tam_vector, int nodo_base);

void resolver_laberinto(mapa laberinto, lista_adyacencia grafo, tabla_busqueda *vector, int num_nodo_Fin);

//Funciones PARA LA COLA
int crea_cola(cola * Q, unsigned int n);

int libera_cola(cola * Q);

int mete_cola(cola * Q, int val);

int saca_cola(cola * Q, int *val);


#endif
