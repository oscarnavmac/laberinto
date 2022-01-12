#include <stdio.h>
#include <stdlib.h>
#include <laberinto.h>
#include <IA_laberinto.h>
#include <ncurses.h>
#include <unistd.h>

int crea_lista_adyacencia(lista_adyacencia *grafo, mapa laberinto)
{
	int i, j;
	grafo->num_nodos = 0;

	grafo->lista = NULL;
	
	for (i = 0; i < laberinto.filas; i++)
	{
		for (j = 0; j < laberinto.columnas; j++)
		{
			if ( (laberinto.matriz[i][j] != '*') )
			{

				if (agrega_nodo(grafo, i, j) == -1)
				{
					//libera_lista_adyacencia(grafo);
					return -1;
				}

				grafo->num_nodos++;
			}
		}
	}

	nodo *apu;

	for (apu = grafo->lista; apu != NULL; apu = apu->sig)
	{
		if ( (laberinto.matriz[modulo(apu->posicion.y - 1, laberinto.filas)][apu->posicion.x] != '*') ) //¿hay nodo arrina?
		{
			apu->arriba = busca_nodo(*grafo, modulo(apu->posicion.y - 1, laberinto.filas), apu->posicion.x); 
		}

		if ( (laberinto.matriz[modulo(apu->posicion.y + 1, laberinto.filas)][apu->posicion.x] != '*') ) // ¿hay nodo abajo?
		{
			apu->abajo = busca_nodo(*grafo, modulo(apu->posicion.y + 1, laberinto.filas), apu->posicion.x); 
		}

		if ( (laberinto.matriz[apu->posicion.y][modulo(apu->posicion.x - 1, laberinto.columnas)] != '*') ) // ¿hay nodo a la izquierda?
		{
			apu->izquierda = busca_nodo(*grafo, apu->posicion.y, modulo(apu->posicion.x - 1, laberinto.columnas)); 
		}

		if ( (laberinto.matriz[apu->posicion.y][modulo(apu->posicion.x + 1, laberinto.columnas)] != '*') ) // ¿hay nodo a la derecha?
		{
			apu->derecha = busca_nodo(*grafo, apu->posicion.y, modulo(apu->posicion.x + 1, laberinto.columnas));
		}		
	}

	return 0;
}

nodo *crea_nodo(int y, int x)
{
	nodo *apu;
	
	apu = (nodo *)malloc(sizeof(nodo));

	apu->posicion.y = y;
	apu->posicion.x = x;
	apu->sig = NULL;
	apu->arriba = NULL;
	apu->abajo = NULL;
	apu->izquierda = NULL;
	apu->derecha = NULL;

	
	
	return apu;
}

int agrega_nodo(lista_adyacencia *grafo, int y, int x)
{
	nodo *nnodo;

	if (( nnodo = crea_nodo(y, x)) != NULL)
	{

		if (grafo->lista == NULL)
			grafo->lista = nnodo;
		else
		{
			nodo *apu;
			
			for (apu = grafo->lista; apu->sig !=NULL; apu = apu->sig);

			apu->sig = nnodo;
		}

		return 0;
	}
	return -1;
}


nodo *busca_nodo(lista_adyacencia grafo, int y, int x)
{
   while (grafo.lista && (grafo.lista->posicion.y != y || grafo.lista->posicion.x != x))
     grafo.lista = grafo.lista->sig;
   return grafo.lista;
}

int modulo(int a, int n)
{
	if (a != -1)
		return a%n;
	else
		return n-1;
}

void libera_lista_adyacencia(lista_adyacencia *grafo)
{
	nodo *apu;

	while (grafo->lista)
	{
		apu = grafo->lista;
		grafo->lista = grafo->lista->sig;
		apu->sig = NULL;
		apu->arriba = NULL;
		apu->abajo = NULL;
		apu->izquierda = NULL;
		apu->derecha = NULL;
	}

	free(grafo->lista);
}

int BFS(tabla_busqueda *vector, int tam_vector, int nodo_base) //vector es la direccion del primer elemnto del arreglo del tipo tabla_busqueda 
{	
	int i;
	int  nodo_adyacente;
	cola Q;

	 if ( crea_cola(&Q, tam_vector + 1) == -1)
		return -1;

	for (i = 0; i < tam_vector; i++)
	{
		vector[i].color = 'B';
		vector[i].paternidad = NULL;
		vector[i].distancia = -1; //infinito
	}

	vector[nodo_base].color = 'G';
	vector[nodo_base].distancia = 0;

	if ( mete_cola(&Q, nodo_base) == -1)
		return -1;
	
	while ( saca_cola(&Q, &nodo_adyacente) != -1)
	{
		for (i = 0; i < tam_vector; i++)
		{
			if ( (vector[nodo_adyacente].nodo->arriba == vector[i].nodo) || (vector[nodo_adyacente].nodo->abajo == vector[i].nodo) ||
				 (vector[nodo_adyacente].nodo->derecha == vector[i].nodo) || (vector[nodo_adyacente].nodo->izquierda == vector[i].nodo) )
			{
				if (vector[i].color == 'B')
				{
					vector[i].paternidad = vector[nodo_adyacente].nodo;
					vector[i].distancia = vector[nodo_adyacente].distancia + 1;
					vector[i].color = 'J';
					if ( mete_cola(&Q, i) == -1)
						return -1;
				}
			}
		}
		//vector[nodo_adyacente].color = 'N';
	}
	
	return libera_cola(&Q);
}

void resolver_laberinto(mapa laberinto, lista_adyacencia grafo, tabla_busqueda *vector, int num_nodo_Fin)
{	
	nodo *camino[vector[num_nodo_Fin].distancia + 1];

	int x = num_nodo_Fin;
	int j = 0, i;

	while (vector[x].paternidad != NULL)
	{
		camino[j] = vector[x].nodo;
		j++;
		for (i = 0; i < grafo.num_nodos; i++)
		{
			if (vector[i].nodo == vector[x].paternidad)
			{
				x = i;
				break;
			}
		}
	}

	initscr();
	noecho();
	curs_set(FALSE);
	
	if(has_colors())
	{
		start_color();/* inicia el modo de colores */
	}
	else
	{
		endwin();
		exit(1);
	}
	
	//DEFINIMOS NUEVO COLOR
	init_color(COLOR_BLUE, 0,0,153);
	init_color(COLOR_BLACK,0,0,0);
	
	
	init_pair( 1, COLOR_WHITE, COLOR_WHITE );
	init_pair( 2, COLOR_BLACK/*Cambia color fuente*/, COLOR_BLACK /*Cambia color fondo*/);
	init_pair( 3, COLOR_YELLOW, COLOR_BLUE );
	init_pair( 4, COLOR_GREEN, COLOR_BLACK);
	init_pair( 5, COLOR_YELLOW, COLOR_BLACK);
	
	
	wbkgd(stdscr, COLOR_PAIR( 3 ));//CAMBIO DE COLOR A FONDO DE PANTALLA
	
	
	WINDOW *ventana ,*secondwin;
	
	nodo *player = NULL;
	int y_matriz, x_matriz;
	int auxiliar_x = 0, auxiliar_y = 0;
	int ancho_ven, altura_ven;
	int y_medio, x_medio;
	int ven_x, ven_y;
	coordenadas centro;
	
	ancho_ven = (laberinto.columnas)/2;
	altura_ven = (laberinto.filas)/2; 
	
	ven_y = 10;
	ven_x = 60;
	ventana = newwin(altura_ven, ancho_ven, ven_y, ven_x);
	
	refresh();
	wrefresh(ventana);
	
	
	secondwin = newwin(altura_ven+2,ancho_ven+2, ven_y - 1, ven_x - 1);
	box(secondwin, 0, 0);
	refresh();
	wrefresh(secondwin);
	
	centro.y = altura_ven/2;
	centro.x = ancho_ven/2;
		
	player = busca_nodo(grafo, laberinto.Inicio.y, laberinto.Inicio.x);

	y_medio = centro.y - player->posicion.y;
	x_medio = centro.x - player->posicion.x;

	for (i = y_medio; i < altura_ven; i++) 
	{
		for (j = x_medio; j < ancho_ven; j++)
		{ 
			if(laberinto.matriz[auxiliar_y][auxiliar_x] =='*')
			{
				wattron(ventana, COLOR_PAIR(1));
				mvwprintw(ventana, i, j, "%c", laberinto.matriz[auxiliar_y][auxiliar_x]);
				wattroff(ventana, COLOR_PAIR(1));
			}
			auxiliar_x++;
		}
		auxiliar_x = 0;
		auxiliar_y++;
	}
	wattron(ventana, COLOR_PAIR(4));
	mvwprintw(ventana, centro.y, centro.x, "o");
	wattroff(ventana, COLOR_PAIR(4));
	
	auxiliar_y = 0;
	auxiliar_x = 0;
	clear();
	box(secondwin,0,0);
	refresh();
	wrefresh(secondwin);
	wrefresh(ventana);
	
	usleep(250000);
  
	for (x = vector[num_nodo_Fin].distancia - 1; x >= 0; x--)
	{	
	
	usleep(250000);

	y_medio = centro.y - camino[x]->posicion.y;
	x_medio = centro.x - camino[x]->posicion.x;
	
	y_matriz = laberinto.filas - (camino[x]->posicion.y - laberinto.Inicio.y);
	x_matriz = laberinto.columnas - (camino[x]->posicion.x - laberinto.Inicio.x);
	
	//Variables de valor constante que se crea sengun el tamaño del mapa para que el ciclo no se pase del limite
	int aux_x = camino[x]->posicion.x + x_medio - laberinto.Inicio.x;
	int aux_y = camino[x]->posicion.y + y_medio - laberinto.Inicio.y;
	
	for (i = y_medio; i < (y_matriz + aux_y); i++) 
	{
		for (j = x_medio; j < (x_matriz + aux_x); j++)
		{	
			//PINTA TODOS LOS ASTERISCOS DE BLANCO
			if(laberinto.matriz[auxiliar_y][auxiliar_x] =='*')
			{
				wattron(ventana, COLOR_PAIR(1));
				mvwprintw(ventana, i, j, "%c", laberinto.matriz[auxiliar_y][auxiliar_x]);
				wattroff(ventana, COLOR_PAIR(1));
			}
			 //PINTA TODOS LOS ESPACIOS DE NEGRO
			if(laberinto.matriz[auxiliar_y][auxiliar_x] ==' ')
			{
				wattron(ventana, COLOR_PAIR(2));
				mvwprintw(ventana, i, j, "%c", laberinto.matriz[auxiliar_y][auxiliar_x]);
				wattroff(ventana, COLOR_PAIR(2));
			}
			//PINTA A 'F' e 'I' de AMARILLO
			if(laberinto.matriz[auxiliar_y][auxiliar_x] =='I' || laberinto.matriz[auxiliar_y][auxiliar_x] =='F')
			{
				wattron(ventana, COLOR_PAIR(5));
				mvwprintw(ventana, i, j, "%c", laberinto.matriz[auxiliar_y][auxiliar_x]);
				wattroff(ventana, COLOR_PAIR(5));
			}
			auxiliar_x++;
		}
		auxiliar_x = 0;
		auxiliar_y++;
	}			
		wattron(ventana, COLOR_PAIR(4));  // COLOREA AL JUGADOR DE VERDE
		mvwprintw(ventana, centro.y, centro.x, "o");
		wattroff(ventana, COLOR_PAIR(4));
		auxiliar_y = 0;
		auxiliar_x = 0;

		clear();
		box(secondwin, 0, 0);
		refresh();
		wrefresh(secondwin);
		wrefresh(ventana);
	
	}  //FIN DE FOR
	
	printw("\t\t\t\t\t\t\tPRESIONE CUALQUIER TECLA PARA SALIR");
	attroff(A_BLINK);
	getch();
	
	clear();
	refresh();
	wrefresh(ventana);
	wrefresh(stdscr);

	endwin();
}

//FUNCIONES PARA LA COLA
int crea_cola(cola * Q, unsigned int n)
{
    Q->N = n;
    Q->arreglo = (int *) malloc (n * sizeof (int));
    if (!Q->arreglo)
    {
         //fprintf (stderr, "Error: No se pudo obtener memoria.\n");
         return -1;
    }
    Q->H = Q->T = 0;
    return 0;
}	

int mete_cola(cola * Q, int val)
{
    if ((Q->T + 1) % Q->N == Q->H)
    {
         //fprintf (stderr, "Error:Cola llena\n");
         return -1;
    }
    Q->arreglo[Q->T] = val;
    Q->T = (Q->T + 1) % Q->N;
    return 0;
}

int saca_cola(cola * Q, int *val)
{
    if (Q->H == Q->T)
    {
         //fprintf (stderr, "Error:Cola Vacia\n");
         return -1;
    }
    *val = Q->arreglo[Q->H];
    Q->H = (Q->H + 1) % Q->N;
    return 0;
}

int libera_cola(cola * Q)
{
    if (Q->arreglo)
    {
         free (Q->arreglo);
         Q->arreglo = NULL;
    }
    Q->H = Q->T = Q->N = 0;
    return 0;
}
