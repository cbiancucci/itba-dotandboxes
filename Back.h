#ifndef Back_h
#define Back_h


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define MIN_PUNTOS 8
#define MAX_PUNTOS 52
#define MAX_JUGADAS_GUARDADAS 2
#define LISO 1
#define MAPA 2
#define SAVE 1
#define JUGADA 2
#define UNDO 3
#define QUIT 4
#define BLOQUE 10
#define CANTIDADJUGADORES 2
#define MAXJUGADAS 4
#define J1VSJ2 0
#define J1VSPC 1
#define PROX_TURNO_J1 1
#define PROX_TURNO_J2_PC 2
#define LINEA_SIN_MARCAR '0'
#define LINEA_MARCADA '1'
#define CUADRO_SIN_MARCAR '0'
#define TIPO_JUEGO 0
#define PROXIMO_TURNO 1
#define FILAS 2
#define COLUMNAS 3
#define CUADRADOS_J1 4
#define CUADRADOS_J2 5
#define MAX_FILENAME 30
#define NORMAL 0
#define RECUPERADO 1
#define BORRA_BUFFER while (getchar() != '\n')

#define VALIDAR_CANT_CUADRADOS(cuadrados, fil, col) ( (cuadrados) <= ((fil)*(col)) ) ? 1:0
#define VALIDAR_CANT_PUNTOS(filas,columnas) ((((filas)+1)*((columnas)+1)) <= MAX_PUNTOS && (((filas)+1)*((columnas)+1)) >= MIN_PUNTOS) ? 1:0
#define CALCULAR_CANT_LINEAS_TABLERO(horizontales,verticales,filas) (((horizontales)*((filas)+1)) + ((verticales)*(filas)))
#define CALCULAR_CANT_CUADRADOS(filas,columnas) ((filas)*(columnas));
#define VALIDAR_PUNTOS_MAX(filas,columnas) ((((filas)+1)*((columnas)+1)) <= MAX_PUNTOS ) ? 1:0
#define VALIDAR_PUNTOS_MIN(filas,columnas) ((((filas)+1)*((columnas)+1)) >= MIN_PUNTOS) ? 1:0

// -------------
// Colores

#define COLOR_RED "\e[31m"
#define COLOR_WHITE "\e[m"

/// ERRORES
#define ERROR1 "No se pudo abrir el archivo.\nCompruebe que el archivo este alojado en el mismo directorio del juego."
#define ERROR2 "El tablero ingresado posee un formato invalido. "
#define ERROR3 "Las dimensiones del tablero ingresado por archivo, son incorrectas."
#define ERROR4 "El archivo esta vacio."
#define ERROR5 "El formato del archivo no corresponde al de una partida guardada."
#define ERROR6 "El formato del archivo no corresponde al de una partida guardada. Las dimensiones del tablero no son validas."
#define ERROR7 "El formato del archivo no corresponde al de una partida guardada. La cantidad de cuadrados no corresponde a las dimensiones del tablero esfecificado en archivo."
#define ERROR8 "De lectura o fin de archivo"
#define ERROR9 "El formato del archivo no corresponde al de una partida guardada. El modo en que se han guardado las posiciones del tablero, es incorrecto."
#define ERROR10 "El formato del archivo no corresponde al de una partida guardada. El modo en que se han guardado los cuadrados del tablero, es incorrecto."
#define ERROR11 "El valor tipoPartida a almacenar es incorrecto."
#define ERROR12 "El valor de turno a almacenar es incorrecto."

// -------------
// Estructuras

typedef struct {
    int cantCuadrados;
    char simbolo;
    char nombre[2];
} tJugador;

typedef struct {
    char ** matrizTablero;
    int filas;
    int columnas;
} tTablero;

/*
	tPartida es la estructura principal del juego. A partir de ella se

	- tipo: dos jugadores o jugador contra la computadora.

	- tipoTablero: liso o mapa.
*/

typedef struct {
    tTablero tablero;
    int tipo;		// Dos jugadores o contra la computadora.
    int tipoTablero; //Liso o Mapa
    int turno;
    tJugador jugadores[2];
    int fin;
    int cuadradosMapa;
} tPartida;

typedef struct {
    int x;
    int y;
    int peso;
} tLinea;

// -------------
// Funciones

int getint(const char mensaje[], ... );

tTablero crearTableroLiso(int filas, int columnas, char * simbolosMatriz);



// Dibuja los simbolos en la matriz
void dibujarPuntos(tTablero tab, char * simbolosMatriz);

// Crea el juego
int crearPartida(tPartida * partidaNueva, int tipoTablero, int cantJugadores, int filas, int columnas, char * simbolosMatriz, char* filename);

// Comando "save filename"
void guardarPartida(char ruta, tPartida partida);

// Recuperar un juego guardado.
int cargarPartida(FILE* arch, tPartida * partida, char simbolosMatriz[]) ;

// Comando "save"
int save(tPartida * partida, const char * filename);

// Comando "quit"
void salir();

// Realiza la jugada en el tablero agregando la linea y validando el o los cuadrados correspondientes.
void jugar(tPartida * miPartida, int extA, int extB);

// Realiza la jugada correspondiente a la computadora.
void juegaPC(tPartida * miPartida);

// Valida que las coordenadas sean correctas para dibujar una linea en el tablero en dicha posicion.
int verificarPosicion(tTablero miTablero, int x, int y);

// Obtiene la posicion de una linea dibujada en el tablero.
int obtenerPosLinea(tTablero miTablero, char extA, char extB, int * x, int * y);

// Realiza el backup de la partida actual, pisando en el vector la jugada actual con la anterior.
void swapPartida(const tPartida origen, tPartida * destino);

//Libera memoria primero de las columnas de cada fila de la matriz, y luego de la matriz total.
void liberarMatrizTablero(tTablero* tab);

#endif
