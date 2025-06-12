#ifndef LABERINTO_H
#define LABERINTO_H

//Esta línea define un nuevo tipo de dato llamado Coordenada
/*typedef sirve para crear un nombre personalizado para un tipo de dato. En este caso, el nuevo tipo se llama Coordenada.
struct no es una función, sino un tipo de dato estructurado que agrupa varias variables (como x e y) bajo un mismo "paquete".
Una Coordenada representa una celda actual o futura que estás visitando durante la generación del laberinto.
Con el DFS (búsqueda en profundidad), cada vez que "explorás" un camino, lo hacés desde una Coordenada.*/
typedef struct {
    int x, y;
} Coordenada;

// Variables globales
extern int filas;
extern int columnas;
extern int modo_explorar;
extern int modo_visual;

// Prototipos de funciones
char** crear_tablero();
char** copiar_tablero(char** original);
void liberar_tablero(char** tablero);

void generar_laberinto(char** tablero);
int calcular_cobertura(char** tablero);
int se_puede_resolver(char** tablero, int x, int y);
int resolver_backtracking(char** tablero, int x, int y, int modo_visual);

void imprimir_tablero(char** tablero);
void limpiar_pantalla();
void explorar_manual(char** tablero);
int leer_entero_rango(const char* mensaje, int min, int max);
int leer_opcion_01(const char* mensaje);

#endif


