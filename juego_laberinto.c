#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <laberinto.h>
#include <jugador_laberinto.h>
#include <IA_laberinto.h>
#include <curses.h>

int main()
{
	int i, key, num_nodo_Inicio, num_nodo_Fin;
	mapa laberinto;
	mapa *apu_laberinto = &laberinto;
	lista_adyacencia grafo;
	lista_adyacencia *apu_grafo = &grafo;
	nodo *nodo_Inicio, *nodo_Fin, *apu_nodo;
	
//	printf("¿Qué nivel quieres? (elige del 1 al 4): ");
	//scanf("%d", &i);
	//crea_matriz_laberinto(i, apu_laberinto);
	//if ( (crea_lista_adyacencia(apu_grafo, laberinto)) != -1)
	initscr();
	clearok(stdscr,TRUE);
	keypad(stdscr, TRUE);
	curs_set(FALSE);

	if(has_colors()) //Comprueba si soparta colores
	{
		start_color(); //Inicia colores
	}
	else
	{
		endwin();
		return -1;
	}
	
	init_color(COLOR_BLACK,0,0,0);
	init_color(COLOR_RED,8000,250,250);
	

	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	
	bkgd(COLOR_PAIR(1));
	

	printw(
			"\n\n\n\n\n\n\n"
			"\t\t ___________________________________________________________________________________________________________  \n"
			"\t\t|                                                                                                           | \n"
			"\t\t|        o88o        o8888o  8888888o  888888o 8888888o   88888888888 888888      888 88888888888  o888888o | \n"
			"\t\t|       o888       88888888 88888888o 8888888 88888888o  88888888888 888 888     888 88888888888 o88888888o | \n"
			"\t\t|      8888       888  888 888   88o 888     888   88o      888     888  888    888     888    8888    8888 | \n"
			"\t\t|     8888       888  888 8888888o  88888o  8888888o       888     888   888   888     888    888      888  | \n"
			"\t\t|    8888       88888888 888888o   88888o  888888o        888     888    888  888     888    888      888   | \n"
			"\t\t|   8888       888  888 888   88o 888     888  88o       888     888     888 888     888    8888    8888    | \n"
			"\t\t|  888888888o 888  888 88888888o 8888888 888   88o  88888888888 888      888888     888     o88888888o      | \n"
			"\t\t| o888888888 888  888 8888888o  888888o 888    88o 88888888888 888       88888     888      o888888o        | \n"
			"\t\t|___________________________________________________________________________________________________________| \n"
			"\n\n");
	attron(COLOR_PAIR(2));
	printw(
			"\t\t\t\t\t\tPulse una de las siguientes teclas...\n"
			"\t\t\t\t\t\t[S] para jugar\n"
			"\t\t\t\t\t\t[B] para ver cómo resolver el laberinto\n\n");
	attroff(COLOR_PAIR(2));
	
	attron(COLOR_PAIR(3) | A_BLINK);
	printw(
			"\t\t\t\t\t\t[PULSE CUALQUIER TECLA PARA SALIR]\n"
			"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
			);
	attroff(COLOR_PAIR(3) | A_BLINK);
			key = getch();
			if (key == 'S' || key == 's' || key == 'B' || key == 'b')
			{
				switch(key)
				{
					case 'S':
					case 's':
						clear();
						refresh();
						mvprintw(10,20,"Qué mapa desea jugar:\n"
						"\t\t\t [Elige del 1-4] : ");
						scanw("%d", &i);
						clearok(stdscr,TRUE);
						endwin();
						crea_matriz_laberinto(i, apu_laberinto);
						if ( (crea_lista_adyacencia(apu_grafo, laberinto)) != -1)
						{
							mover_jugador(laberinto, grafo);
						}
						libera_lista_adyacencia(apu_grafo);
						libera_matriz_laberinto(apu_laberinto);
				break;
				case 'B':
				case 'b':
					 clear();
						refresh();
						mvprintw(10,20,"Qué mapa desea resolver:\n"
						"\t\t\t [Elige del 1-4] : ");
						scanw("%d", &i);
						clearok(stdscr,TRUE);
						endwin();
						crea_matriz_laberinto(i, apu_laberinto);
						if ( (crea_lista_adyacencia(apu_grafo, laberinto)) != -1)
						{
							tabla_busqueda vector[grafo.num_nodos];

							nodo_Inicio = busca_nodo(grafo, laberinto.Inicio.y, laberinto.Inicio.x);
							nodo_Fin = busca_nodo(grafo, laberinto.Fin.y, laberinto.Fin.x);
							apu_nodo = grafo.lista;

							for (i = 0; i < grafo.num_nodos; i++)
							{
								vector[i].nodo = apu_nodo;
								if (vector[i].nodo == nodo_Inicio)
									num_nodo_Inicio = i;
								if (vector[i].nodo == nodo_Fin)
									num_nodo_Fin = i;
								apu_nodo = apu_nodo->sig;
							}

							if ( BFS(vector, grafo.num_nodos, num_nodo_Inicio) != -1)
								resolver_laberinto(laberinto, grafo, vector, num_nodo_Fin);
						}
						libera_lista_adyacencia(apu_grafo);
						libera_matriz_laberinto(apu_laberinto);
				default:
				break;
				}
			}
	

//	libera_lista_adyacencia(apu_grafo);
//	libera_matriz_laberinto(apu_laberinto);
	
	endwin();
	return 0;
}
