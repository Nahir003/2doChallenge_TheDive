#include "laberinto.h"
#include <stdio.h> //entrada y salida estándar: printf,scanf etc.
#include <time.h> // necesario para time() para inicializar rand() con srand(time(NULL)).
#include <windows.h> // para Sleep()
#include <stdlib.h> /*para funciones de manejo de memoria dinámica como malloc(), calloc() y free() y generar números aleatorios con rand, srand.
                    Memory ALLOCation reservar un espacio de memoria dinámicamente en el heap (montón), en tiempo de ejecución.
                    free Sirve para liberar la memoria que reservaste con malloc.(Ya no necesito este espacio. Podés reciclarlo
                    calloc Reserva y pone todo en 0.)*/
#include <string.h> //Trae funciones como memcpy, strcpy, strcmp

int filas = 21, columnas = 21;
int modo_explorar = 1;
int modo_visual = 1;

int main() {
    srand(time(NULL));
    
    printf("╔══════════════════════════════════════╗\n");
    printf("║         EL LABERINTO MAESTRO         ║\n");
    printf("╚══════════════════════════════════════╝\n\n");

    // Leer tamaño del laberinto
    printf(" CONFIGURACIÓN INICIAL:\n");
    filas = leer_entero_rango("Tamaño del laberinto (5-99): ", 5, 99);
    if (filas % 2 == 0) filas++; // Asegurar que sea impar para el patrón
    columnas = filas;
    
    printf("\nGenerando laberinto de %dx%d...\n", filas, columnas);
    
    char** tablero = crear_tablero();
    
    // Generar laberinto con patrón clásico
    generar_laberinto(tablero);
        
    // Colocar entrada y salida
    tablero[1][1] = 'E';
    tablero[filas - 2][columnas - 2] = 'S';
    
    // Verificar que sea resoluble
    char** copia = copiar_tablero(tablero);
    int resuelto = se_puede_resolver(copia, 1, 1);
    liberar_tablero(copia);
    
    int cobertura = calcular_cobertura(tablero);
    
    printf("Laberinto generado exitosamente!\n");
    printf("Estadísticas: Cobertura %d%%, %s\n", 
           cobertura, resuelto ? "resoluble" : "verificando...");
    
    printf("\n═══  LABERINTO GENERADO ═══\n");
    printf("⚫ = Entrada  🏁 = Salida  🟪 = Paredes  🔲 = Caminos\n\n");
    imprimir_tablero(tablero);
    
    printf("\n OPCIONES DE JUEGO:\n");
    modo_explorar = leer_opcion_01(" ¿Explorar manualmente primero? [1] Sí  [0] No\n➤ ");
    modo_visual = leer_opcion_01(" ¿Ver resolución paso a paso? [1] Sí  [0] No\n➤ ");
    
    if (modo_explorar) {
        explorar_manual(tablero);
    }
    
    printf("\n RESOLUCIÓN AUTOMÁTICA:\n");
    printf(" Iniciando algoritmo de backtracking...\n");
    
    // Medir tiempo de resolución
    LARGE_INTEGER start, end, freq; 
    QueryPerformanceFrequency(&freq); //pregunta cuantos ticks por seg tiene el reloj
    QueryPerformanceCounter(&start); //inicia el contador y lo guarda
    
    char** tablero_solucion = copiar_tablero(tablero);// Crea una copia exacta del laberinto original.
    int exito = resolver_backtracking(tablero_solucion, 1, 1, modo_visual);
    
    QueryPerformanceCounter(&end); //Guarda el contador final
    double tiempo = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart; 
    //end - start te da cuántos ticks pasaron, Dividido por freq, te da el tiempo en segundos
    
    printf("\n═══ RESULTADOS ═══\n");
    if (exito) {
        printf(" ¡Laberinto resuelto exitosamente!\n");
        printf(" Tiempo de resolución: %.6f segundos\n", tiempo);
        
        if (!modo_visual) {
            printf("\n═══  SOLUCIÓN FINAL ═══\n");
            printf("🔳 = Camino de la solución\n\n");
            imprimir_tablero(tablero_solucion);
        }
    } else {
        printf("No se pudo resolver el laberinto.\n");
    }
    
    // Evita memory leaks, libera la matriz original y la copia de solución.
    liberar_tablero(tablero);
    liberar_tablero(tablero_solucion);
    
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║     ¡Gracias por usar Laberinto      ║\n");
    printf("║      Maestro! Presiona Enter...      ║\n");
    printf("╚══════════════════════════════════════╝\n");
    getchar();// Espera que el usuario vea el mensaje antes de cerrar
    
    return 0;
}