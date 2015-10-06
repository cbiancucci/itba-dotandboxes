//dotBoxesFront.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Back.h"


static void imprimirMenu();
static void imprimirTablero(tTablero miTablero);
int validarFormatoPunto(const char* ext);
static void pedirComando(tPartida * vectorPartidas, int * flagUndo, int * flagSalir);
static void pedirTipoTablero(int * tipoTablero);
static void pedirFilasColumnas(int * filasCuadrados, int * columnasCuadrados);
int generarPartida(tPartida * vectorPartidas , int cantidadJugadores, char * simbolosMatriz);
static void iniciarPartida(tPartida * vectorPartidas, int tipo, int * flagSalir);
static void recuperarJuegoGrabado(tPartida * vectorPartidas, char simbolosMatriz[], int * flagSalir);
static void pedirFilename(char filename[], const char[]);
static int validarFormatoFilename(char filename[], const char extension[]);
static void imprimirError(int nroError);

static void imprimirError(int nroError)
{
    char * error = malloc(300*sizeof(char));;
    switch (nroError)
    {
    case 1:
        error = ERROR1;
        break;
    case 2:
        error = ERROR2;
        break;
    case 3:
        error = ERROR3;
        break;
    case 4:
        error = ERROR4;
        break;
    case 5:
        error = ERROR5;
        break;
    case 6:
        error = ERROR6;
        break;
    case 7:
        error = ERROR7;
        break;
    case 8:
        error = ERROR8;
        break;
    case 9:
        error = ERROR9;
        break;
    case 10:
        error = ERROR10;
        break;
    case 11:
        error = ERROR11;
        break;
    case 12:
        error = ERROR12;
        break;
    }
    printf("Error %d: %s \n\n", nroError, error);
    free(error);
}


int main() {

    int opcion, resp = 0;
    int flagSalir = 0;
    char simbolosMatriz[52] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                               's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
                               'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
                              };

    while (!flagSalir) {
        imprimirMenu();
	opcion = getint(" Opcion: ");
        tPartida vectorPartidas[MAXJUGADAS];
        switch(opcion) {
        case 1:
		printf("\n\n -----------------------------------------------------------------------------\n\n");
		printf("\t\t\t Jugador Vs. Computadora.\n\n");

		resp = generarPartida(vectorPartidas, J1VSPC, simbolosMatriz);

		if(resp == 1)
	           iniciarPartida(vectorPartidas, NORMAL, &flagSalir);
		break;

        case 2:
		printf("\n\n -----------------------------------------------------------------------------\n\n");
		printf("\t\t\t Dos Jugadores.\n\n");

		resp = generarPartida(vectorPartidas, J1VSJ2, simbolosMatriz);

		if(resp == 1)
	           iniciarPartida(vectorPartidas, NORMAL, &flagSalir);
		break;

        case 3:
		printf("\n\n -----------------------------------------------------------------------------\n\n");
		printf("\t\t\t Recuperar un juego grabado.\n\n");
		recuperarJuegoGrabado(vectorPartidas, simbolosMatriz, &flagSalir);
		break;

        case 4:
		flagSalir = 1;
		break;

        default:
		printf(COLOR_RED " Error: opcion ingresada invalida.\n\n" COLOR_WHITE);
		break;
        }
    }

    system("clear");
    return 0;
}



static void imprimirMenu()
{
    printf("\n\n ##############################################################################");
    printf("\n #####################\t\t\t\t\t ######################");
    printf("\n #######\t\t\t Dot & Boxes \t\t\t\t#######");
    printf("\n #####################\t\t\t\t\t ######################");
    printf("\n ##############################################################################\n\n");

    printf(" Menu:\n");
    printf(" -----\n\n");
    printf(" 1- Un jugador Vs. Computadora.\n");
    printf(" 2- Dos jugadores.\n");
    printf(" 3- Recuperar un juego grabado.\n");
    printf(" 4- Salir.\n");
}

static void imprimirTablero(tTablero miTablero)
{
    int i, j;

    printf("\n\n");

    for(i = 0; i < miTablero.filas; i++)
    {
        printf("\t\t\t");

        for(j = 0; j < miTablero.columnas; j++)
        {
            printf("%c", miTablero.matrizTablero[i][j]);
            printf(" ");
        }

        printf("\n");
    }

    printf("\n\n");
}

/*
	validarFormatoPunto se encarga de validar la dimension del comando jugada.
*/

int validarFormatoPunto(const char* ext)
{
    if( (strlen(ext)) != 1)
        return 0;
    return 1;
}

/*
	Maquina de Estados: ingreso de comandos.

	- save filename: guarda el juego en la direccion filename.

	- undo: volvera una jugada atras. Posee como restriccion el no poder repetir esta accion inmediatamente.

	- quit: saldra del juego. Al salir, pregunta si se desea guardar el juego.
*/

static void pedirComando(tPartida * vectorPartidas, int * flagUndo, int * flagSalir)
{
	int c, longitud, validaOK, pedir = 1, i;
	char comando[4], filename[FILENAME_MAX] = "", salir;
	int estado = 0, extA = 0, extB = 0, cantComando, resp;
	char ext1[1], ext2[1];


	if( (*flagUndo > 0) && (*flagUndo <= 4) )
		(*flagUndo)--;

	while(pedir)
    	{
        printf(" Ingrese comando: ");
        scanf("%s", comando);
        printf("\n");

        if( (longitud = strlen(comando)) > 0 && longitud < 5)
        {
	    // Consumo el siguiente caracter al string escaneado y guardado en comando.
            c = getchar();

            if( (c == ' ') && ( strstr(comando, "save") != NULL ) )
	    {
		// Leo hasta el final de linea el filename.
		for(i = 0; ( c = getchar() ) != '\n' ; i++)
			filename[i] = c;

		estado = SAVE;
	    }
            
            else if( strstr(comando, "undo") != NULL )
                estado = UNDO;
            else if( strstr(comando, "quit") != NULL )
                estado = QUIT;
            else if( strrchr(comando, '-') != NULL )
            {
		cantComando = sscanf(comando, "%c-%c", ext1, ext2);
				
		if (cantComando == 2 && strlen(comando) == 3)
		{
			estado = JUGADA;
		}
                else
		{
                    printf(COLOR_RED " Error: Debe ingresar dos puntos del tablero con un formato valido.\n");
		    printf(" Sugerencia: punto1-punto2.\n\n" COLOR_WHITE);
		}
            }
            else
	    {
                printf(COLOR_RED " Error: Ha ingresado un comando invalido.\n");
		printf(" Sugerencias: punto1-punto2 | undo | save filename | quit\n\n" COLOR_WHITE);
	    }
            switch(estado)
            {
                case SAVE:

                            resp = save(&vectorPartidas[3], filename);

                            if (resp != 0)                            
                            	imprimirError(resp);
                            else
				{
	                            printf(" Partida guardada satisfactoriamente.\n\n");
				    pedir = 0;
				}
                            break;
                case UNDO:
			    if( (*flagUndo) == 0)
                            {
		                    swapPartida(vectorPartidas[0], &vectorPartidas[3]);
		                    (*flagUndo) = 4;
		                    pedir = 0;
                            }
                            else
                            {
                            	printf(COLOR_RED " No tiene permitido realizar el comando: %s\n\n" COLOR_WHITE, comando);
                            }
                            break;
                case QUIT:
                            printf(" ¿Desea guardar antes de salir? (S/N): ", comando);
			    salir = getchar();
			    if ( (salir == 's') || (salir == 'S') )
			    {
				printf("Por favor, ingrese nombre de archivo: ");
				scanf("%s",filename);
			        save(&vectorPartidas[3], filename);
			        pedir = 0;
			        (*flagSalir) = 1;
			    }
			    else if ( (salir == 'n') || (salir == 'N') )
			    {
			        pedir = 0;
			        (*flagSalir) = 1;
			    }
			    else
			        printf(COLOR_RED "\n Error: comando invalido.\n" COLOR_WHITE);
                            break;
                case JUGADA:
                            if(!obtenerPosLinea(vectorPartidas[3].tablero, *ext1, *ext2, &extA, &extB))
                                printf(COLOR_RED "\n No es posible agregar la linea.\n\n" COLOR_WHITE);
                            else
                            {
                                if(verificarPosicion(vectorPartidas[3].tablero, extA, extB))
                                {
                                    jugar(&vectorPartidas[3], extA, extB);
                                    pedir = 0;
                                }
                                else
                                    printf(COLOR_RED "\n Error: La linea ya existe.\n" COLOR_WHITE);
                            }
                            break;
            }
        }
        else
	{
            printf(COLOR_RED " Error: Ha ingresado un comando invalido.\n");
	    printf(" Sugerencias: punto1-punto2 | undo | save filename | quit\n\n" COLOR_WHITE);
	}
    }
}

/*
	Funcion adicional que se encarga de pedir el nombre del jugador.
	Si la misma recibe un solo jugador, llamara al Jugador 2 como PC.
	Dicha funcion retorna un vector de jugadores con los datos iniciales.

	REVISAR: de verdad vale la pena? hay una validacion de nombre en el medio, me parece al recontra pedo.
*/



static void pedirTipoTablero(int * tipoTablero)
{
    int tipoTableroAux;
    int flagOK = 0;

    do {

        printf(" ------------------------------------------------\n\n");
        printf(" Seleccione el tipo de tablero: \n\n");
        printf(" 1- Liso\n");
        printf(" 2- Mapa\n\n");
        tipoTableroAux = getint(" Opcion: ");
        printf("\n");

        switch(tipoTableroAux)
	{
		case LISO:
			(*tipoTablero) = tipoTableroAux;
			flagOK = 1;
			break;

		case MAPA:
			(*tipoTablero) = tipoTableroAux;
			flagOK = 1;
			break;

		default:
			printf(COLOR_RED " Error: Opcion elegida invalida.\n\n" COLOR_WHITE);
			break;
        }

    } while(!flagOK);

    printf(" Opcion %d seleccionada.\n\n",(*tipoTablero));
}

/*
	pedirFilasColumnas es la encargada de pedir al usuario un formato valido de filas y columnas.
	Una vez verificado que los datos ingresados son correctos, se asignan a los punteros recibidos.
*/

static void pedirFilasColumnas(int * filasCuadrados, int * columnasCuadrados)
{
    int filas, columnas, cantDimensiones;
    int flagOK = 0;

    char stringfilasColumnas[3];


    do {

        printf(" ------------------------------------------------\n\n");
        printf("\n Ingrese las dimensiones del tablero: ");

	scanf("%s", stringfilasColumnas);
	cantDimensiones = sscanf(stringfilasColumnas,"%dx%d",&filas,&columnas);

        if (cantDimensiones != 2)
        {
            printf(COLOR_RED "\n Error: formato ingresado invalido.\n" COLOR_WHITE);
            printf(COLOR_RED "\n Ejemplo: 2x3 - 3x4 - 5x5.\n\n" COLOR_WHITE);
        }
	else
	{
            if( VALIDAR_CANT_PUNTOS(filas, columnas) )
            {
                (*filasCuadrados) = filas;
                (*columnasCuadrados) = columnas;
                flagOK = 1;
            }
            else
                printf(COLOR_RED "\n Error: dimension ingresada invalida.\n" COLOR_WHITE);
        }

    } while(!flagOK);
}

static void pedirFilename(char filename[], const char extension[])
{
    int flag = 0;

    do
    {
        printf(" Ingrese el nombre del archivo: ");
        scanf("%s", filename);
        flag = validarFormatoFilename(filename, extension);

    }while(!flag);

}

static int validarFormatoFilename(char filename[], const char extension[])
{
    char* separador=".";
    char* particion;
    char aux[MAX_FILENAME] = "";
    char* name = NULL;
    char* ext = NULL;

    //Luego de utilizar strtock en validarFormatoFilename se destruye el string. Para no perder filename, usamos un aux.
    strcpy(aux,filename);

    name = strtok(aux, separador);    // Primera llamada => Primer token

    while( (particion = strtok(NULL, separador)) != NULL )   // Posteriores llamadas
    {
        if(strstr(particion, separador)==NULL) //Para saltear el separador del medio
            ext = particion;
    }

    // Si no encuentro error, retorno 1.
    if( (name != NULL) &&  (ext != NULL) &&  ( strstr(ext, extension) != NULL ) )
        return 1;

    // Si encontre algun error, retorno 0.
    printf(COLOR_RED "\n Error: Formato no corresponde al nombre de un archivo.\n");
    printf(" Sugerencia: filename.%s \n\n" COLOR_WHITE, extension);

    return 0;
}

/*
	generarPartida tiene como objetivo recibir los parametros establecidos en la seleccion del juego y crear la partida que se utilizara a lo 		largo de la ejecucion del programa.

	Como el programa incluye la funcionalidad de regresar movimientos hacia atras, se trabaja con un vector de tipo tPartida. En el se van 		almacenando los movimientos progresivos alternados de cada jugador (o pc).

	Al generar la partida, este vector se inicializa en cada posicion con la partida inicial generada.
*/

int generarPartida(tPartida * vectorPartidas , int cantidadJugadores, char * simbolosMatriz)
{
    int i=0, resp = 0, tipoTablero, filasCuadrados = 0, columnasCuadrados = 0;
    char filename[MAX_FILENAME];
    char extension[] = "map";

    pedirTipoTablero(&tipoTablero);

    if ( tipoTablero==LISO )
        pedirFilasColumnas(&filasCuadrados, &columnasCuadrados);

    else if ( tipoTablero == MAPA )
        pedirFilename(filename, extension);


    for (i = 0; (i < MAXJUGADAS) && (resp == 0); i++)
    {
        resp = crearPartida(&vectorPartidas[i], tipoTablero, cantidadJugadores, filasCuadrados, columnasCuadrados, simbolosMatriz,filename);
        if(resp != 0)
            imprimirError(resp);
    }

    return (resp ==  0) ? 1 : 0;
}


/*
	iniciarPartida se encarga de establecer los turnos y llamar a las funciones de jugada, tales como "juegaPC" o "pedirComando".

	Antes de comenzar cada turno, se realiza un backup de los atributos anteriores, haciendo uso de la funcion "swapPartida".

*/
static void iniciarPartida(tPartida * vectorPartidas, int tipo, int * flagSalir)
{

    int turno, flagUndo = 0;

	if(vectorPartidas[3].turno < 0)
		vectorPartidas[3].turno = (vectorPartidas[3].turno*(-1))-1;
	else
		vectorPartidas[3].turno = (rand() % CANTIDADJUGADORES);

    do {

        if(vectorPartidas[2].turno != vectorPartidas[3].turno)
        {
            swapPartida(vectorPartidas[1], &vectorPartidas[0]);
            swapPartida(vectorPartidas[2], &vectorPartidas[1]);
            swapPartida(vectorPartidas[3], &vectorPartidas[2]);
        }

        turno = (vectorPartidas[3].turno % CANTIDADJUGADORES);

        imprimirTablero(vectorPartidas[3].tablero);

        printf(" %s, su turno. \n", vectorPartidas[3].jugadores[turno].nombre);

        if(!strcmp(vectorPartidas[3].jugadores[turno].nombre, "PC"))
	{
		juegaPC(&vectorPartidas[3]);

		if( (flagUndo > 0) && (flagUndo <= 4) )
			flagUndo--;
	}
	else
		pedirComando(vectorPartidas, &flagUndo, flagSalir);

    } while( (!vectorPartidas[3].fin) && ((*flagSalir) == 0) );

    if((*flagSalir) == 0)
    {
        imprimirTablero(vectorPartidas[3].tablero);

        if(vectorPartidas[3].jugadores[0].cantCuadrados > vectorPartidas[3].jugadores[1].cantCuadrados)
            printf(" Gano %s !!!", vectorPartidas[3].jugadores[0].nombre);
        else if (vectorPartidas[3].jugadores[0].cantCuadrados < vectorPartidas[3].jugadores[1].cantCuadrados)
            printf(" Gano %s !!!", vectorPartidas[3].jugadores[1].nombre);
        else
            printf(" Empataron, jueguen la revancha!!");
    }
}


/*
	REVISAR: todo.
	recuperarJuegoGrabado es el encargado de leer un archivo, descrifrar sus atributos, y luego generar una
*/

static void recuperarJuegoGrabado(tPartida * vectorPartidas, char simbolosMatriz[], int * flagSalir)
{
    char fileName[MAX_FILENAME];
    int i = 0;
    FILE* arch;

	printf("Por favor, ingrese nombre de archivo: ");
	scanf("%s",fileName);

    arch = fopen(fileName,"r");

    if (arch == NULL)
    {
        printf(COLOR_RED " Error: No se pudo abrir el archivo. \n" COLOR_WHITE);
        return;
    }
    tPartida * partida = calloc(1, sizeof(tPartida));
    int resp = cargarPartida(arch, partida, simbolosMatriz); // Llamo al back con el archivo.
    fclose(arch);

    if (resp != 0)
        imprimirError(resp);
    else
    {
        for (i = 0; i < MAXJUGADAS; i++)
            vectorPartidas[i] = *partida;

        iniciarPartida(vectorPartidas, RECUPERADO, flagSalir);
    }

    free(partida);

}

