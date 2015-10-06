#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Back.h"


static void dibujarCuadradoMapa(tTablero * tab, int x, int y);
static void calcularPosLinea(int contador, int* colLinea, int* filaLinea, int* cambioLinea, int *pasoAVertical, int cantHorizPorFila, int cantVertPorFila);
static int lineasActualesPorCuadrado(tTablero miTablero, int i, int j);

static void
calcularPosLinea(int contador, int* colLinea, int* filaLinea, int* cambioLinea, int *pasoAVertical, int cantHorizPorFila, int cantVertPorFila)
{

    if(contador==(*cambioLinea))
    {

        (*filaLinea)++;

        /*Si pasoAVertical, significa que el actual y los proximos (cantVertPorFila-1) elementos en el archivo
        representaran lineas verticales */
        if(*pasoAVertical)
        {
            /*Las lineas verticales se encuentran en las posiciones de columnas pares de nuestra matrizTablero*/
            *colLinea=0;
            /*La proxima vez que i==cambioLinea se pasara a evaluar lineas horizontales.*/
            *pasoAVertical=0;
            /*El proximo cambio ocurrira luego de analizar los proximos (cantVertPorFila-1) elementos en el archivo*/
            (*cambioLinea)+=cantVertPorFila;
        }
        /*Sino, significa que el actual y los proximos (cantHorizontales-1) elementos en el archivo
        representaran lineas horizontales*/
        else
        {
            /*Las lineas horizontales se encuentran en las posiciones de columnas impares de nuestra matrizTablero*/
            *colLinea=1;
            /*La proxima vez que i==cambioLinea se pasara a evaluar lineas verticales.*/
            *pasoAVertical=1;
            /*El proximo cambio ocurrira luego de analizar los proximos (cantHorizPorFila) elementos en el archivo*/
            (*cambioLinea)+=cantHorizPorFila;

        }
    }
    /*Si no hay cambio de tipo de linea y no es la primera pasada. (i!=0)*/
    else if(contador!=0)
    {
        /*Sea que se este analizando lineas verticales u horizontales, su posicion tiene una diferencia de 2
        en la matrizTablero.*/
        (*colLinea)+=2;
    }

}

int getint(const char mensaje[], ... )
{
	int n, salir = 0;
	va_list ap;

	do
	{
		va_start(ap, mensaje);
		vprintf(mensaje, ap);
		va_end(ap);

		if ( scanf("%d",&n) != 1)
		{
			BORRA_BUFFER;
			return -1;
		}
		else
			salir = 1;
	} while (!salir);
	BORRA_BUFFER;
	return n;
}


/*
	dibujarPuntos se encarga de recorrer el tablero y completar los puntos (extremos de juego) con los simbolos que contiene el vector de simbolos 		para la matriz.

	Se opto por la creacion de esta funcion, teniendo en cuenta que a futuro el programa puede ser modificado y dichos puntos reemplazados por 		otros simbolos sin la necesidad de modificar la funcion que los genera. Con solo modificar el vector de los simbolosMatriz, le funcion se 		encarga de posicionarlos en el tablero.

	Todo aquello que no pertence a los extremos se rellena con un espacio. A lo largo de la ejecucion del juego, se ira modificando por la 		existencia de lineas y/o cuadrados.
*/

void dibujarPuntos(tTablero tab, char * simbolosMatriz)
{
    int i, j, posEnVec = 0;

    for(i = 0; i < tab.filas; i++)
    {
        for(j = 0; j < tab.columnas; j++)
        {
                if( (i%2) == 0 && (j%2) == 0)
                {
                    tab.matrizTablero[i][j] = simbolosMatriz[posEnVec];
                    posEnVec++;
                }
                else if( (tab.matrizTablero[i][j] != '-') && (tab.matrizTablero[i][j] != '|') && (tab.matrizTablero[i][j] !='#') && (tab.matrizTablero[i][j] !='&') && (tab.matrizTablero[i][j]!='$') )
                    tab.matrizTablero[i][j] = ' ';
        }
    }
}


/*
	dibujarLinea recibe el tablero y una posicion del mismo. Dependiendo de la misma, escribe en dicha posicion, una linea horizontal o una 	vertical.
*/

static void dibujarLinea(tTablero miTablero, int x, int y)
{
    if( (y%2) == 1 )
        miTablero.matrizTablero[x][y] = '-';
    else
        miTablero.matrizTablero[x][y] = '|';
}




/*
	crearTableroLiso recibe las dimensiones y los simbolos (puntos o extremos) que van a conformar el tablero en cuestion.

	Dicho tablero es en reaildad una matriz de char, la cual se completara con simbolos, lineas y cuadrados a lo largo de la ejecucion del juego.
	Los simbolosMatriz son aquellos que representaran los extremos entre los cuales el jugador debera ingresar comandos para dibujar lineas. Este 		vector de simbolos puede ser modificado desde el front y asi customizar el tablero.

	Las dimensiones de la matriz estan dadas de la siguiente forma: el usuario ingresa la cantidad de filas y columnas, es decir, la cantidad de 		cuadrados que tendra el tablero. Sin embargo, la matriz requiere mayor tamano para alojar los simbolos, las lineas y los cuadrados.
	Es por este motivo, que la dimension de la matriz se calcula de la siguiente forma:

	3x3 = (2*3)+1 ; (2*3)+1

	    a   -   b    -    c   -   d
	    |	X   |    X    |   X   |
	    e   -   f    -    g   -   h
	    |	X   |    X    |   X   |
	    i   -   j    -    k   -   l
	    |	X   |    X    |   X   |
	    m   -   n    -    o   -   p

	Dado que la matrizTablero es una matriz dinamica, es decir, depende de las dimensaiones ingresadas y a partir de alli se reserva memoria, se 		realiza un malloc por cantidad de filas, y luego se recorre cada fila realizando un malloc por cantidad de columnas.

	Luego se llama a dibujarPuntos para que se encargue de completar las posiciones correspondientes con los simbolosMatriz.
*/

tTablero crearTableroLiso(int filas, int columnas, char * simbolosMatriz)
{
    int i = 0;

    tTablero tab;
    tab.filas = (filas*2) + 1;
    tab.columnas = (columnas*2) + 1;


    tab.matrizTablero = malloc(tab.filas * sizeof(char *));

    for (i = 0; i < tab.filas; i++) {
        tab.matrizTablero[i] = malloc(tab.columnas * sizeof(char));
    }

    dibujarPuntos(tab, simbolosMatriz);

    return tab;
}

static void dibujarCuadradoMapa(tTablero * tab, int x, int y)
{
	tab->matrizTablero[x][y] = '#';
	tab->matrizTablero[x+1][y] = '-';
	tab->matrizTablero[x-1][y] = '-';
	tab->matrizTablero[x][y+1] = '|';
	tab->matrizTablero[x][y-1] = '|';
}

/*
	crearTableroMapa es la funcion encargada de leer un archivo mapa, guardado anteriormente con datos de una partida, y generar la misma en la 		ultima instancia en que se guardo.
*/

static int crearTableroMapa(tTablero * tab, char * simbolosMatriz, char* filename,int* cantCuadradosMapa)
{
	tLinea * posCuadrado, *temp;
    FILE* archMapa;
    char c;
    int i, j, filas = 0, columnas = 0, primeraVez = 1;

    archMapa = fopen(filename,"rt");
    if(archMapa == NULL)
    {
        fclose(archMapa);
        return 1;
    }

    c = fgetc(archMapa);

    if(!feof(archMapa)&&c!='\n')
    {
        /*Se recorren las filas del archivo*/
        for(i = 0; !feof(archMapa); i++)
        {
            for(j = 0; !feof(archMapa) && c != '\n'; j++)
            {
                if(c =='#')
                {
		    if (*cantCuadradosMapa == 0)
                        posCuadrado = malloc((BLOQUE) * sizeof(tLinea));
		    (posCuadrado)[*cantCuadradosMapa].x = (i*2)+1;
                    (posCuadrado)[*cantCuadradosMapa].y = (j*2)+1;
                    (*cantCuadradosMapa)++;
                    if(((*cantCuadradosMapa)%BLOQUE) == 0) {
                        temp = realloc((posCuadrado), ((*cantCuadradosMapa)+BLOQUE) * sizeof(tLinea));
                        if (temp != NULL)
                            (posCuadrado) = temp;
                    }
                }
                else if(c != '0')
                {
                    fclose(archMapa);
                    return 2;
                }

		 c = fgetc(archMapa);
            }
//Termina de recorrer columnas
            if(primeraVez)
            {
                columnas = j;
                primeraVez = 0;
            }

            /*Si la cantidad de columnas en la fila es distinta a la primera o la cantidad de filas y/o columnas supera a MAX_PUNTOS.*/
            else if( j!=columnas || !VALIDAR_PUNTOS_MAX(i, j) )
            {
                fclose(archMapa);
                return 3;
            }

            /*Para recorrer las columnas de la nueva fila.*/
            j = 0;

            c = fgetc(archMapa);


        }/*Termina de recorrer fila.*/

        filas = i;

	*tab = crearTableroLiso(filas, columnas, simbolosMatriz);
	for(i = 0; i < (*cantCuadradosMapa); i++)
	{
		dibujarCuadradoMapa(tab, (posCuadrado)[i].x, (posCuadrado)[i].y);
	}
	
        fclose(archMapa);

	for (i = 0; i < tab->filas; i++) {
		for(j = 0; j < tab->columnas; j++)
		{
			// si los dos son impares (mod = 1 + mod = 1) = 2 (ES POS DE CUADRADO)
			if((((i%2) + (j%2)) == 2) && (tab->matrizTablero[i][j] != '#')) 
			{
				if(lineasActualesPorCuadrado(*tab, i, j) == 4)
				{
					tab->matrizTablero[i][j] = '#';
					(*cantCuadradosMapa)++;
				}
			}
		}
	}

        return 0;

    }/*Si entra en else, el archivo estaba vacio.*/
    else
    {
        fclose(archMapa);
        return 4;
    }

}



void liberarMatrizTablero(tTablero* tab)
{
    int i;

    for(i=0;i<tab->filas;i++)
    {
        free(tab->matrizTablero[i]);
    }

}

static void crearJugadores(tJugador * jugadores, int cantJugadores)
{
	tJugador jugador1, jugador2;
	jugador1.simbolo = '&';
	strcpy(jugador1.nombre, "J1");
	jugador1.cantCuadrados = 0;
	jugadores[0] = jugador1;

	jugador2.simbolo = '$';
	if(cantJugadores == J1VSJ2)
		strcpy(jugador2.nombre, "J2");
	else if(cantJugadores == J1VSPC)
		strcpy(jugador2.nombre, "PC");
	jugador2.cantCuadrados = 0;
	jugadores[1] = jugador2;
}

// Crea la partida. Me viene el tipo de tablero, liso o mapa. Los jugadores del front y su cantidad, etc.
int crearPartida(tPartida * partidaNueva, int tipoTablero, int cantJugadores, int filas, int columnas, char * simbolosMatriz, char* filename) {

    tTablero tablero;
    int cantCuadradosMapa=0;
    partidaNueva->fin = partidaNueva->turno = partidaNueva->cuadradosMapa = 0;

    crearJugadores(partidaNueva->jugadores, cantJugadores);

    partidaNueva->tipoTablero = tipoTablero;
	// J1VSPC J1VSJ2
    partidaNueva->tipo = cantJugadores;

    if(tipoTablero == LISO){

        tablero = crearTableroLiso(filas, columnas, simbolosMatriz); // Creo el tablero con los simbolos
        partidaNueva->tablero = tablero;
    }
    else if (tipoTablero == MAPA) {
        int resp = crearTableroMapa(&tablero,simbolosMatriz,filename,&cantCuadradosMapa);
        if(resp == 0)
        {
            partidaNueva->tablero = tablero;
	    partidaNueva->cuadradosMapa = cantCuadradosMapa;
        }
        else
            return resp;
    }

    return 0;
}

// Verifica que un extremo sea "conectable" con otro. Es decir, si el extremo est\E1 alrededor.
static int estaExtremo(tTablero miTablero,  char ext, int i, int j, int * x, int * y) {
    // La validaci\F3n es larga, ya que esta funcion, valida primero si la linea es del contorno y mira hacia el lado correspondiente.
    // Sino, mira si es interior y en ese aso si el extremo est\E1 en algna de las 4 direcciones.
    if((i == 0) && (j == 0)) {
        if(miTablero.matrizTablero[i][j+2] == ext) {
            *x = i;
            *y = j+1;
            return 1;
        } else if(miTablero.matrizTablero[i+2][j] == ext) {
            *x = i+1;
            *y = j;
            return 1;
        }
    } else if ((i == 0) && (j == miTablero.columnas-1)) {
        if(miTablero.matrizTablero[i][j-2] == ext) {
            *x = i;
            *y = j-1;
            return 1;
        } else if(miTablero.matrizTablero[i+2][j] == ext) {
            *x = i+1;
            *y = j;
            return 1;
        }
    } else if ((i == miTablero.filas-1) && (j == 0)) {
        if(miTablero.matrizTablero[i][j+2] == ext) {
            *x = i;
            *y = j+1;
            return 1;
        } else if(miTablero.matrizTablero[i-2][j] == ext) {
            *x = i-1;
            *y = j;
            return 1;
        }
    } else if ((i == miTablero.filas-1) && (j == miTablero.columnas-1)) {
        if(miTablero.matrizTablero[i][j-2] == ext) {
            *x = i;
            *y = j-1;
            return 1;
        } else if(miTablero.matrizTablero[i-2][j] == ext) {
            *x = i-1;
            *y = j;
            return 1;
        }
    } else {
        if((j != 0) && (miTablero.matrizTablero[i][j-2] == ext)) {
            *x = i;
            *y = j-1;
            return 1;
        } else if((i != 0) && (miTablero.matrizTablero[i-2][j] == ext)) {
            *x = i-1;
            *y = j;
            return 1;
        } else if((i != miTablero.filas-1) && (miTablero.matrizTablero[i+2][j] == ext)) {
            *x = i+1;
            *y = j;
            return 1;
        } else if((j != miTablero.columnas-1) && (miTablero.matrizTablero[i][j+2] == ext)) {
            *x = i;
            *y = j+1;
            return 1;
        }
    }
    return 0;
}

// Verifica que exista un extremo, en ese caso busca el otro.
int obtenerPosLinea(tTablero miTablero, char extA, char extB, int * x, int * y) {
    int i = 0, j = 0;
    for (i = 0; i < miTablero.filas; i+=2) {
        for (j = 0; j < miTablero.columnas; j+=2) {
            if(miTablero.matrizTablero[i][j] == extA)
                return estaExtremo(miTablero,  extB, i, j, x, y);
            else if(miTablero.matrizTablero[i][j] == extB)
                return estaExtremo(miTablero,  extA, i, j, x, y);
        }
    }
    return 0;
}

// Verifica que no haya una linea y que no sea el lugar del cuadrado.
int verificarPosicion(tTablero miTablero, int x, int y) {
    if((miTablero.matrizTablero[x][y] == ' ') &&
            (!(((x%2) == 1) && ((y%2) == 1)))) // Que no sea la pos del simbolo Matriz[impar][impar]
        return 1;
    else
        return 0;
}


// Cuenta cuantas lineas rodea a un cuadrado.
static int lineasActualesPorCuadrado(tTablero miTablero, int i, int j) {
    int cantLineas = 0;
    if(miTablero.matrizTablero[i][j+1] != ' ')
        cantLineas++;
    if(miTablero.matrizTablero[i][j-1] != ' ')
        cantLineas++;
    if(miTablero.matrizTablero[i+1][j] != ' ')
        cantLineas++;
    if(miTablero.matrizTablero[i-1][j] != ' ')
        cantLineas++;
    return cantLineas;
}

// Si hago x,y y es un cuadrado, ponele el simbolo y sumame puntos.
static int hayCuadrado(tTablero miTablero, tJugador * jugador, int x, int y) {
    if(lineasActualesPorCuadrado(miTablero, x, y) == 4) {
        miTablero.matrizTablero[x][y] = jugador->simbolo;
        jugador->cantCuadrados++;
        return 1;
    }
    return 0;
}

// Retorna el valor de los posibles cuadrados. ya que de acerdo a la posici\F3n del primer cuadrado puede ser uno solo, o ese m\E1s otro que sea hacia arriba, abajo, derecha o izquierda.
static void posiblesCuadrados(int * fil, int * col, int * filCuadradoII, int * colCuadradoII, int cantFilas, int cantColumnas) {
    if (*fil == 0) // Borde Superior
        (*fil)++;
    else if(*fil == cantFilas) // Borde Inferior
        (*fil)--;
    else if(*col == 0) // Columna Izq
        (*col)++;
    else if(*col == cantColumnas) // Columna Der
        (*col)--;
    else { // Interno
        if(((*col%2) == 1)  && ((*fil%2) == 0)) {
            *colCuadradoII = *col;
            *filCuadradoII = (*fil) - 1;
            (*fil)++;
        } else if(((*col%2) == 0)  && ((*fil%2) == 1)) {
            *filCuadradoII = *fil;
            *colCuadradoII = (*col) + 1;
            (*col)--;
        }
    }
}

// Verifico los posibles cuadrados respecto a la linea que voy a agregar. Luego, veo si hay cuadrados.
static int verificarCuadrados(tPartida * miPartida, int fil, int col) {
    int filCuadradoII = 0, colCuadradoII = 0;
    posiblesCuadrados(&fil, &col, &filCuadradoII, &colCuadradoII, ((miPartida->tablero.filas)-1), ((miPartida->tablero.columnas)-1));

    int turno = (miPartida->turno % CANTIDADJUGADORES);
    int cuadrado = hayCuadrado(miPartida->tablero, &miPartida->jugadores[turno], fil, col);

    if(filCuadradoII != 0 && colCuadradoII != 0)
        cuadrado += hayCuadrado(miPartida->tablero, &miPartida->jugadores[turno], filCuadradoII, colCuadradoII);

    return cuadrado;
}

static void calcularPosSimbolo(const int columnas, const int contador, int* filaSimbolo, int* colSimbolo) {
    //Si se inicia una nueva fila
    if((contador%columnas)==0) {

        if(contador==0)
            *filaSimbolo=1;
        /*Si no es la primera fila, porque filaSimbolo tiene que ser 1.
        Se guarda en filaAux la posicion de la nueva fila del simbolo de cuadrado en el tablero.*/
        else
            *filaSimbolo+=2;

        *colSimbolo=1;//Se cuentan las columnas desde la primera posicion nuevamente.
    } else //Se sigue en una misma fila, se avanza en columna.
        *colSimbolo+=2;//Se guarda en colSimbolo la posicion de la nueva columna del simbolo de cuadrado en el tablero.

}

static int validarOpcion(int tipo, int opcion, tPartida * partida, int * filas, int * columnas) {
    switch(tipo) {
    case TIPO_JUEGO:
        if(opcion==J1VSJ2)
        {
            partida->tipo = J1VSJ2;
            strcpy(partida->jugadores[1].nombre, "Jug 2");
        }
        else if( opcion==J1VSPC )
        {
            partida->tipo = J1VSPC;
            strcpy(partida->jugadores[1].nombre, "PC");
        }
        else
            return 5;
        break;

    case PROXIMO_TURNO:
        if(opcion==PROX_TURNO_J1 || opcion==PROX_TURNO_J2_PC )
            partida->turno = -opcion ;
        else
            return 5;
        break;

    case FILAS:
        *filas=opcion;//Cuando se crea el tablero, se setea la verdadera dimension de "partida.tablero.filas"
        break;

    case COLUMNAS:
        if(VALIDAR_CANT_PUNTOS(*filas,opcion))
            *columnas=opcion;//Cuando se crea el tablero, se setea la verdadera dimension de "partida.tablero.columnas"
        else
            return 6;
        break;

    case CUADRADOS_J1:
        partida->jugadores[0].cantCuadrados = opcion;
        break;

    case CUADRADOS_J2:
        if( VALIDAR_CANT_CUADRADOS((opcion)+(partida->jugadores[0].cantCuadrados), *filas, *columnas))
            partida->jugadores[1].cantCuadrados = opcion;

       else {
            free(partida->jugadores);
            return 7;
        }
        break;
    }
    return 0;
}

static int evaluarOpcionesEnteras(FILE* arch, tPartida ** miPartida, int * filas, int * columnas) {
    int tipo, opcion, resp;
    for(tipo=0; tipo<6; tipo++) { //Se eval\FAan los primeros 6 enteros almacenados en el archivo.
        resp = fread(&opcion, sizeof(opcion),1, arch);
		if (resp != 1)
		    return 8;
        resp = validarOpcion(tipo, opcion, *miPartida, filas, columnas);
        if(resp != 0)
            return resp;
    }
    return 0;
}

static int dibujarLineasDesdeArchivo(FILE* arch, tPartida * partida, int filas, int columnas, char *c) {

    int i, filaLinea=0,colLinea=1,cantHorizPorFila,cantVertPorFila,cantLineasTablero,pasoAVertical=1,cambioLinea;


    /*Se calculan la cantidad de lineas horizontales y verticales por linea*/
    cantHorizPorFila = columnas;
    cantVertPorFila = columnas+1;

    /*Controla en que pasada del ciclo for se pasa de recibir datos de filas horizontales a verticales y viceversa*/
    cambioLinea = cantHorizPorFila;

    /*Se calcula la cantidad de Lineas posibles en el tablero*/
    cantLineasTablero = CALCULAR_CANT_LINEAS_TABLERO(cantHorizPorFila, cantVertPorFila, filas);

    /*Se toma del archivo el primer dato de la tira de chars correspondientes a linea*/
    *c = fgetc(arch);

    /*Se recorren todas las posibles lineas a dibujar en matrizTablero representadas en el archivo con un 0 o un 1*/
    for(i=0; i<cantLineasTablero; i++)
    {

        /*Mientras que no se haya llegado a fin de archivo (tadavia deberia quedar en el archivo la tira de chars
        correspondientes a cuadrados), ni se encuentre un salto de linea, y el dato evaluado sea un 1 o un 0.*/
        if( !feof(arch) && *c!='\n' && (*c==LINEA_SIN_MARCAR || *c==LINEA_MARCADA) )
        {
            calcularPosLinea(i,&colLinea,&filaLinea,&cambioLinea,&pasoAVertical,cantHorizPorFila, cantVertPorFila);

            /*Si en el archivo se paso un 1 significa que hay que dibujar una linea. En caso contrario, se ignora.*/
            if(*c==LINEA_MARCADA)
            {
                dibujarLinea(partida->tablero,filaLinea,colLinea);
            }

            /*Se pide el siguiente elemento del archivo, el cual representa lineas del tablero*/
            *c = fgetc(arch);


        }
        /*Si llego a fin de archivo, se encontro un salto de linea, o el dato evaluado no es 1 ni 0.
        Significa que el archivo posee un formato invalido*/
        else
        {
            return -1;
        }

    }/*Termina ciclo for.*/

    return 0;

}




static int dibujarCuadradosDesdeArchivo(FILE* arch, int cantCuadrados, char * c, int columnas, tPartida * partida) {

    int filaSimbolo, colSimbolo, i;
    for(i=0; i<cantCuadrados; i++) {
        calcularPosSimbolo(columnas, i, &filaSimbolo, &colSimbolo);
        if( !feof(arch) && (*c==CUADRO_SIN_MARCAR || *c=='#' || *c=='&' || *c=='$') ) {
            if(*c=='#') {
                partida->tipoTablero = MAPA;
                partida->tablero.matrizTablero[filaSimbolo][colSimbolo]='#';
                partida->cuadradosMapa += 1;
            } else if(*c=='&')
                partida->tablero.matrizTablero[filaSimbolo][colSimbolo]='&';

            else if(*c=='$')
                partida->tablero.matrizTablero[filaSimbolo][colSimbolo]='$';
        } else
            return -1;
        *c = fgetc(arch);
    }
    return 0;
}

int cargarPartida(FILE* arch, tPartida * partida, char simbolosMatriz[]) {

    int filas,columnas, cantCuadrados, resp = 0;
    char c;
        partida->jugadores[0].simbolo = '&';
    partida->jugadores[1].simbolo = '$';
    strcpy(partida->jugadores[0].nombre, "J1");

    resp = evaluarOpcionesEnteras(arch, &partida, &filas, &columnas);

    if(resp != 0) // Ya tengo guardado el error
        return resp;

    if(partida->tipo==J1VSJ2)
        strcpy(partida->jugadores[1].nombre, "J2");
    else
        strcpy(partida->jugadores[1].nombre, "PC");

    /*Por default se crea un tablero liso de dimensiones especificadas en archivo.
    En caso de encontrar un '#' en la secuencia de chars del archivo que alberga
    las posiciones de cuadrados, se cambia tipoTablero a MAPA.*/
    partida->tablero = crearTableroLiso(filas,columnas,simbolosMatriz);
    partida->tipoTablero = LISO;
    partida->fin = 0;

    resp = dibujarLineasDesdeArchivo(arch, partida, filas, columnas, &c);
    if(resp != 0) 
        return 9;
    //FIN: Evaluacion de secuencia de chars correspondiente a lineas del tablero

    //Evaluacion de secuencia de chars correspondiente a cuadrados.
    //Modularizar: dibujarCuadradosDesdeArchivo OJO! no perder el ultimo getc del anterior for de lineas.

    cantCuadrados = CALCULAR_CANT_CUADRADOS(filas,columnas);
    if(cantCuadrados > 0) {
                resp = dibujarCuadradosDesdeArchivo(arch, cantCuadrados, &c, columnas, partida);
        if(resp != 0)
            return 10;
    }
    //FIN: Evaluacion de secuencia de chars correspondiente a cuadrados.
    return 0;
}

// Juego en la partida con la linea que representa los extremos.
void jugar(tPartida * miPartida, int extA, int extB) {
    dibujarLinea(miPartida->tablero, extA, extB); // Dibujo la linea. Paso turno porque si hace cuadrado, le pogo el simbolo.
    if(verificarCuadrados(miPartida, extA, extB) == 0) // Si la linea que agrego no hizo cuadrados, agrego uno a turno
        miPartida->turno++;
    // Si la suma de cuadrados es igual a la cantidad de cuadrados, termino el juego.
    miPartida->fin = ((miPartida->jugadores[0].cantCuadrados + miPartida->jugadores[1].cantCuadrados + miPartida->cuadradosMapa) == ((miPartida->tablero.columnas-1)/2 * (miPartida->tablero.filas-1)/2)) ? 1 : 0;
}

static int esCandidata(int i, int j, tTablero miTablero, int * cuadradosPosibles) {
    int candidata = 1,   x = i, y = j, filCuadradoII = -1, colCuadradoII = -1, lineasCuadradoI = 0, lineasCuadradoII = 0;
    *cuadradosPosibles = 0;
    // Si es del contorno, verifico el cuadrado  correspondiente.
    // Si es interna y vertica, verifico  el cuadrado izquierdo y derecho
    // Si es Interna y horizontal, verifico el cuadrado de arriba y el cuadrado de abajo.
    posiblesCuadrados(&x, &y, &filCuadradoII, &colCuadradoII, ((miTablero.filas)-1), ((miTablero.columnas)-1)); // Cuadrados que limitan con la linea
    lineasCuadradoI = lineasActualesPorCuadrado(miTablero, x, y); // Uno SIEMPRE
    if((filCuadradoII != -1) && (colCuadradoII != -1))
        lineasCuadradoII = lineasActualesPorCuadrado(miTablero, filCuadradoII, colCuadradoII); // DOS SI ES INTERNA

    if(((lineasCuadradoI == 2 && lineasCuadradoII == 3) || (lineasCuadradoII == 2 && lineasCuadradoI == 3)) || (lineasCuadradoI == 3 && lineasCuadradoII == 3)) {
        // Si uno tiene 2 lineas y el otro tiene 3 o los dos tienen 3. Rankeo a la linea con 2, pues agrega esta linea, hace el cuadrado de 3 y en la proxima pasada, va a ser candidata la linea que falta
        candidata = 1;
        (*cuadradosPosibles)= 2;
    } else if(lineasCuadradoI == 2 || lineasCuadradoII == 2) // Si los dos tienen dos lineas, no ser\E1 candidata, pues  agregue donde agregue le dejar\E9 hacer un cuadrado.
        candidata = 0;
    else if((lineasCuadradoI == 3) || (lineasCuadradoII == 3)) { // Si alguno tiene 3  es candidata, pero podr\E1 hacer un solo cuadrado.
        candidata = 1;
        (*cuadradosPosibles) = 1;
    }
    return candidata;
}

static int posiblesMovimientos(tTablero miTablero, tLinea ** lineasPosibles) {
    int i = 0, j = 0, cantidad = 0, cuadradosPosibles = 0, cand = 0;
    tLinea * temp;

    for (i = 0; i < miTablero.filas; i++) {
        for (j = 0; j < miTablero.columnas; j++) {
            if ((((((i%2) == 0) && ((j%2) == 1)) || (((i%2) == 1) && ((j%2) == 0))) && (miTablero.matrizTablero[i][j] == ' '))) { // Es una linea posible a dibujar.
                cand = esCandidata(i, j, miTablero, &cuadradosPosibles);
                if(cand == 1) {
                    if (cantidad == 0)
                        *lineasPosibles = malloc((BLOQUE) * sizeof(tLinea));
                    (*lineasPosibles)[cantidad].x = i;
                    (*lineasPosibles)[cantidad].y = j;
                    (*lineasPosibles)[cantidad].peso = cuadradosPosibles;
                    cantidad++;
                    if((cantidad%BLOQUE) == 0) {
                        temp = realloc((*lineasPosibles), ((cantidad)+BLOQUE) * sizeof(tLinea));
                        if (temp != NULL)
                            (*lineasPosibles) = temp;
                    }
                }
            }
        }
    }
	if(cantidad > 0)
	{
	    temp = realloc(*lineasPosibles, (cantidad) * sizeof(tLinea));
	    if (temp != NULL)
		*lineasPosibles = temp;
	}
    return cantidad;
}

static int mejorCandidata(tLinea * lineasPosibles, int capacidad) {
    int pos = (rand() % capacidad), mejor = 0,  i = 0;
    if(capacidad == 1)
        return 0;

    for(i = 0; i < capacidad; i++) {
        if(lineasPosibles[i].peso > mejor) {
            mejor = lineasPosibles[i].peso;
            pos = i;
        }
    }

    return pos;
}

void juegaPC(tPartida * miPartida) {

    tLinea * lineasPosibles, * temp;
    int pos = 0, cantidad = 0, i = 0, j = 0;
    int cantidadPosibles = posiblesMovimientos(miPartida->tablero, &lineasPosibles);
    if(cantidadPosibles == 0) {
        lineasPosibles = malloc((BLOQUE) * sizeof(tLinea));
        for (i = 0; i < miPartida->tablero.filas; i++) {
            for (j = 0; j < miPartida->tablero.columnas; j++) {
                if (((((i%2) == 0) && ((j%2) == 1)) || (((i%2) == 1) && ((j%2) == 0))) && (miPartida->tablero.matrizTablero[i][j] == ' ')) { // Es una linea posible a dibujar.
                    lineasPosibles[cantidad].x = i;
                    lineasPosibles[cantidad].y = j;
                    lineasPosibles[cantidad].peso = 0;
                    cantidad++;
                    if((cantidad%BLOQUE) == 0) {
                        temp = realloc(lineasPosibles, ((cantidad)+BLOQUE) * sizeof(tLinea));
                        if (temp != NULL)
                            lineasPosibles = temp;
                    }
                }
            }
        }
        pos = (rand() % cantidad);
    } else
        pos = mejorCandidata(lineasPosibles, cantidadPosibles);

    jugar(miPartida, lineasPosibles[pos].x, lineasPosibles[pos].y);
    free(lineasPosibles);
}

/*
	swapPartida se utiliza para ir guardando parcialmente la partida en un vector de tipo tPartida.
	Su funcionalidad principal es recorrer la tPartida y copiar todos sus datos a una posicion anterior del vector.
	De esta forma, al finalizar cada turno, se tiene una copia de los movimientos (y demas atributos) anteriores.

	swapPartida(origen, destino)
*/

void swapPartida(const tPartida origen, tPartida * destino)
{
    int i, j;

    destino->fin = origen.fin;
    destino->tipo = origen.tipo;
    destino->tipoTablero = origen.tipoTablero;
    destino->turno = origen.turno;
    destino->cuadradosMapa = origen.cuadradosMapa;
    destino->tablero.columnas = origen.tablero.columnas;
    destino->tablero.filas = origen.tablero.filas;

    for(i = 0; i < CANTIDADJUGADORES; i++)
    {
	(destino->jugadores)[i].cantCuadrados = origen.jugadores[i].cantCuadrados;
	(destino->jugadores)[i].simbolo = origen.jugadores[i].simbolo;
	strcpy((destino->jugadores)[i].nombre, origen.jugadores[i].nombre);
    }

    for(i = 0; i < origen.tablero.filas; i++) {
        for(j = 0; j < origen.tablero.columnas; j++) {
            destino->tablero.matrizTablero[i][j] = origen.tablero.matrizTablero[i][j];
        }
    }
}



/*** SAVE ***/

int save(tPartida * partida, const char * filename)
{
	FILE* arch;
	int filas, columnas, i, j, turno;

	filas = (partida->tablero.filas-1)/2;
	columnas = (partida->tablero.columnas-1)/2;

	turno = (partida->turno % CANTIDADJUGADORES)+1;

	arch = fopen(filename,"wb");

	if(arch == NULL)
		return 8;
    	if(partida->tipo == J1VSJ2 || partida->tipo == J1VSPC)
        	fwrite(&(partida->tipo), sizeof(int),1, arch);
    	else
		//Esto es interno. Por si alguien en algun momento cambia los valores.
		//Aqui se indica que hay una incoherencia respecto de como se guarda el archivo.
		return 11;

    	if(turno == PROX_TURNO_J1 || turno == PROX_TURNO_J2_PC)
        	fwrite(&turno, sizeof(int),1, arch);

	else
		/*Esto es interno. Por si alguien en algun momento cambia los valores.*/
		//Aqui se indica que hay una incoherencia respecto de como se guarda el archivo.
		return 12;

	fwrite(&filas, sizeof(filas),1, arch);
	fwrite(&columnas, sizeof(int),1, arch);
	fwrite(&(partida->jugadores[0].cantCuadrados), sizeof(int),1, arch);
	fwrite(&(partida->jugadores[1].cantCuadrados), sizeof(int),1, arch);
	//Impresion de las lineas del tablero en el archivo.
	for(i = 0; i < partida->tablero.filas; i++)
	{
		for(j = 0; j < partida->tablero.columnas; j++)
		{

			/*Recorre filas pares columnas impares. Son las coordenadas de lineas horizontales */
			if( (i%2) == 0 && (j%2) == 1)
			{
				if(partida->tablero.matrizTablero[i][j] == '-')
                fputc('1', arch);
				else
				    fputc('0', arch);
			}

			/*Recorre filas impares columnas pares. Son las coordenadas de lineas verticales */
			if( (i%2) == 1 && (j%2) == 0)
			{
				if(partida->tablero.matrizTablero[i][j] == '|')
                fputc('1', arch);
				else
				    fputc('0', arch);

			}

		}
	}

	// Impresion de los cuadrados del tablero en el archivo.
	for(i = 1; i < partida->tablero.filas; i = i+2)
	{
		for(j = 1; j < partida->tablero.columnas; j = j+2)
		{

			if(partida->tablero.matrizTablero[i][j] == ' ') // Si no hay cuadrado.
				fputc('0', arch);
			else
				fputc(partida->tablero.matrizTablero[i][j], arch); // Si hay cuadrado (J1, J2/PC, o mapa bloqueado).
		}
	}
	fclose(arch);
	return 0;
}
