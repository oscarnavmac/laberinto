#include <stdlib.h>
#include <time.h>
#include <laberinto.h>
#include <jugador_laberinto.h>
#include <IA_laberinto.h>
#include <ncurses.h>
#include <unistd.h>

void mover_jugador(mapa laberinto, lista_adyacencia grafo)
{
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
	
	nodo *player = NULL, *final = NULL;
	int i, j;
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
	final = busca_nodo(grafo, laberinto.Fin.y, laberinto.Fin.x);

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
  
	int movimiento;
	
	
	keypad(stdscr, TRUE);
 
 	while ( (movimiento = getch()) != 27)
	{	
		system("clear");
		switch(movimiento)
		{
			case KEY_RIGHT:
				if (player->derecha)
				{				
					player = player->derecha;
				}
				break;

			case KEY_LEFT:
				if (player->izquierda)
				{
					player = player->izquierda;
				}
				break;

			case KEY_UP:
				if (player->arriba)
				{
					player = player->arriba;
				}
				break;

			case KEY_DOWN:
				if (player->abajo)
				{
					player = player->abajo;
				}
				break;
		}
	y_medio = centro.y - player->posicion.y;
	x_medio = centro.x - player->posicion.x;
	
	y_matriz = laberinto.filas - (player->posicion.y - laberinto.Inicio.y);
	x_matriz = laberinto.columnas - (player->posicion.x - laberinto.Inicio.x);
	
	//Variables de valor constante que se crea sengun el tamaño del mapa para que el ciclo no se pase del limite
	int aux_x = player->posicion.x + x_medio - laberinto.Inicio.x;
	int aux_y = player->posicion.y + y_medio - laberinto.Inicio.y;
	
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
	
		if(player == final)
		{
			printw(
								"\n"
								"\t\t\t\t\t88  88  o88o     o88888   o88o  o88  88  o88o  8888o   o888o  \n"
								"\t\t\t\t\t88  88 88  88   88       88  88 8888 88 88  88 88  88 88   88 \n"
								"\t\t\t\t\t888888 88  88   88  888o 88  88 88 8888 88  88 88  88 8     8 \n"
								"\t\t\t\t\t88  88 888888   88    88 888888 88  888 888888 88  88 88   88 \n"
								"\t\t\t\t\t88  88 88  88    o88888o 88  88 88   8o 88  88 8888o   o888o  \n\n"
							);
			attron(A_BLINK);
			printw("\t\t\t\t\t\t\tPRESIONE [ESC] PARA SALIR");
			attroff(A_BLINK);
		}
	
	}  //FIN DE WHILE
	
	clear();
	refresh();
	wrefresh(ventana);
	wrefresh(stdscr);

	endwin();
}


