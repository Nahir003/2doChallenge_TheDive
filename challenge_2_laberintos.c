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

//Esta línea define un nuevo tipo de dato llamado Coordenada
/*typedef sirve para crear un nombre personalizado para un tipo de dato. En este caso, el nuevo tipo se llama Coordenada.
struct no es una función, sino un tipo de dato estructurado que agrupa varias variables (como x e y) bajo un mismo "paquete".
Una Coordenada representa una celda actual o futura que estás visitando durante la generación del laberinto.
Con el DFS (búsqueda en profundidad), cada vez que "explorás" un camino, lo hacés desde una Coordenada.*/
typedef struct {
    int x, y;
} Coordenada;

/* Un puntero es una variable que guarda una dirección de memoria se representa con *
 un puntero doble es un puntero que apunta a otro puntero se representa con **  */

/*Malloc= Permite crear matrices dinámicas y grandes sin reventar el stack.
Puede definir el tamaño del laberinto en tiempo de ejecución, retornarlo desde funciones y usar más memoria (heap), liberándola después con free().*/

void generar_laberinto_clasico(char** tablero);
int leer_entero_rango(const char* mensaje, int min, int max);

char** crear_tablero() {
    char** tablero = (char**) malloc(filas * sizeof(char*)); //Reserva memoria para un array de punteros a char, uno por cada fila.
    for (int i = 0; i < filas; i++) { // Bucle para crear cada fila por separado. Cada fila es un array de char.
        tablero[i] = (char*) malloc(columnas * sizeof(char)); 
        for (int j = 0; j < columnas; j++) {
            tablero[i][j] = '#'; // Inicializar todo como paredes
        }
    }
    return tablero;
}

void liberar_tablero(char** tablero) {
    for (int i = 0; i < filas; i++) {
        free(tablero[i]);
    }
    free(tablero);
}

char** copiar_tablero(char** original) {
    char** copia = crear_tablero();
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            copia[i][j] = original[i][j];
        }
    }
    return copia;
}

void barajar_array(int* arr, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void generar_laberinto_clasico(char** tablero) {
    // Stack para DFS iterativo
    Coordenada* stack = (Coordenada*) malloc((filas * columnas) * sizeof(Coordenada));
    int stack_top = 0;
    
    // Matriz de visitados para las celdas (solo posiciones impares)
    int** visitado = (int**) malloc(filas * sizeof(int*));
    for (int i = 0; i < filas; i++) {
        visitado[i] = (int*) calloc(columnas, sizeof(int));
    }
    
    // Comenzar desde (1,1) - primera celda válida
    int start_x = 1, start_y = 1;
    stack[stack_top++] = (Coordenada){start_x, start_y};
    visitado[start_x][start_y] = 1;
    tablero[start_x][start_y] = '*'; // Marcar como camino
    
    // Direcciones: arriba, abajo, izquierda, derecha (saltos de 2)
    int dx[] = {-2, 2, 0, 0};
    int dy[] = {0, 0, -2, 2};
    
    while (stack_top > 0) {
        Coordenada actual = stack[--stack_top];
        int x = actual.x, y = actual.y;
        
        // Encontrar vecinos no visitados
        int vecinos_validos[4];
        int num_vecinos = 0;
        
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            // Verificar que esté dentro de los límites y en posición impar
            if (nx > 0 && ny > 0 && nx < filas - 1 && ny < columnas - 1 && 
                nx % 2 == 1 && ny % 2 == 1 && !visitado[nx][ny]) {
                vecinos_validos[num_vecinos++] = i;
            }
        }
        
        if (num_vecinos > 0) {
            // Volver a poner la celda actual en la pila
            stack[stack_top++] = actual;
            
            // Elegir un vecino aleatorio
            int dir_elegida = vecinos_validos[rand() % num_vecinos];
            int nx = x + dx[dir_elegida];
            int ny = y + dy[dir_elegida];
            
            // Marcar el vecino como visitado y como camino
            visitado[nx][ny] = 1;
            tablero[nx][ny] = '*';
            
            // Abrir la pared entre la celda actual y el vecino
            int pared_x = x + dx[dir_elegida] / 2;
            int pared_y = y + dy[dir_elegida] / 2;
            tablero[pared_x][pared_y] = '*';
            
            // Agregar el vecino a la pila
            stack[stack_top++] = (Coordenada){nx, ny};
        }
    }
    
    // Liberar memoria
    free(stack);
    for (int i = 0; i < filas; i++) {
        free(visitado[i]);
    }
    free(visitado);
}

void agregar_algunos_caminos_extra(char** tablero) {
    // Agregar algunos caminos adicionales para hacer el laberinto menos linear
    // Esto es opcional y se hace con moderación
    int caminos_extra = (filas * columnas) / 50; // Muy pocos caminos extra
    
    for (int i = 0; i < caminos_extra; i++) {
        int x = (rand() % (filas - 2)) + 1;
        int y = (rand() % (columnas - 2)) + 1;
        
        // Solo convertir paredes que tengan al menos 2 caminos adyacentes
        if (tablero[x][y] == '#') {
            int vecinos_camino = 0;
            int dx[] = {-1, 1, 0, 0};
            int dy[] = {0, 0, -1, 1};
            
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (nx >= 0 && ny >= 0 && nx < filas && ny < columnas && 
                    tablero[nx][ny] == '*') {
                    vecinos_camino++;
                }
            }
            
            if (vecinos_camino >= 2 && rand() % 100 < 15) { // 15% de probabilidad
                tablero[x][y] = '*';
            }
        }
    }
}

int calcular_cobertura(char** tablero) {
    int caminos = 0;
    int total_celdas = 0;
    
    for (int i = 1; i < filas - 1; i++) {
        for (int j = 1; j < columnas - 1; j++) {
            total_celdas++;
            if (tablero[i][j] == '*' || tablero[i][j] == 'E' || tablero[i][j] == 'S') {
                caminos++;
            }
        }
    }
    
    return total_celdas > 0 ? (caminos * 100) / total_celdas : 0;
}

void limpiar_pantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void imprimir_tablero(char** tablero) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            switch (tablero[i][j]) {
                case '#': printf("🟪"); break;  // Paredes
                case '*': printf("🔲"); break;  // Caminos
                case 'E': printf("⚫"); break;  // Entrada
                case 'S': printf("🏁"); break;  // Salida
                case '-': printf("👤"); break;  // Jugador
                case '+': printf("🔳"); break;  // Camino de solución
                default:  printf("  "); break;
            }
        }
        printf("\n");
    }
}

int resolver_backtracking(char** tablero, int x, int y, int modo_visual) {
    if (x < 0 || x >= filas || y < 0 || y >= columnas) return 0;
    
    if (tablero[x][y] == 'S') return 1;
    
    if (tablero[x][y] != '*' && tablero[x][y] != 'E') return 0;
    
    char original = tablero[x][y];
    if (tablero[x][y] != 'E') tablero[x][y] = '+';
    
    if (modo_visual) {
        limpiar_pantalla();
        printf("Resolviendo paso a paso...\n");
        imprimir_tablero(tablero);
        Sleep(100);
    }
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (resolver_backtracking(tablero, nx, ny, modo_visual)) {
            return 1;
        }
    }
    
    if (original != 'E') tablero[x][y] = original;
    return 0;
}

int se_puede_resolver(char** tablero, int x, int y) {
    if (x < 0 || x >= filas || y < 0 || y >= columnas) return 0;
    if (tablero[x][y] == 'S') return 1;
    if (tablero[x][y] != '*' && tablero[x][y] != 'E') return 0;
    
    char temp = tablero[x][y];
    tablero[x][y] = 'x'; // Marcar como visitado
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if (se_puede_resolver(tablero, nx, ny)) {
            tablero[x][y] = temp;
            return 1;
        }
    }
    
    tablero[x][y] = temp;
    return 0;
}

void explorar_manual(char** tablero) {
    int x = 1, y = 1;
    char input;
    
    printf("\n=== EXPLORACIÓN MANUAL ===\n");
    printf("Controles: W(arriba) S(abajo) A(izquierda) D(derecha) Q(salir)\n");
    printf("Objetivo: Llegar a la bandera 🏁\n\n");
    
    do {
        limpiar_pantalla();
        printf("=== EXPLORACIÓN MANUAL ===\n");
        printf("Controles: WASD para mover, Q para salir\n");
        printf("Posición actual: (%d, %d)\n\n", x, y);
        
        char temp = tablero[x][y];
        tablero[x][y] = '-';
        imprimir_tablero(tablero);
        tablero[x][y] = temp;
        
        printf("\n Tu movimiento: ");
        scanf(" %c", &input);
        
        int nx = x, ny = y;
        switch (input) {
            case 'w': case 'W': nx--; break;
            case 's': case 'S': nx++; break;
            case 'a': case 'A': ny--; break;
            case 'd': case 'D': ny++; break;
        }
        
        if (nx >= 0 && ny >= 0 && nx < filas && ny < columnas &&
            (tablero[nx][ny] == '*' || tablero[nx][ny] == 'S')) {
            x = nx;
            y = ny;
        } else if (input != 'q' && input != 'Q') {
            printf("No podés moverte ahí. Presiona Enter...");
            while (getchar() != '\n');
            getchar();
        }
        
        if (tablero[x][y] == 'S') {
            limpiar_pantalla();
            char temp = tablero[x][y];
            tablero[x][y] = '-';
            imprimir_tablero(tablero);
            tablero[x][y] = temp;
            printf("\n FELICITACIONES! ¡Encontraste la salida manualmente!\n");
            printf(" Sos todo un explorador de laberintos.\n");
            printf("Presiona Enter para continuar con la resolución automática...");
            while (getchar() != '\n');
            getchar();
            break;
        }
        
    } while (input != 'q' && input != 'Q');
    
    if (input == 'q' || input == 'Q') {
        printf("\nSaliste de la exploración manual.\n");
    }
}

int leer_entero_rango(const char* mensaje, int min, int max) {
    char entrada[20];
    int valor;
    
    while (1) {
        printf("%s", mensaje);
        if (fgets(entrada, sizeof(entrada), stdin) != NULL) {
            if (sscanf(entrada, "%d", &valor) == 1 && valor >= min && valor <= max) {
                return valor;
            }
        }
        printf("Entrada inválida. Ingresá un número entre %d y %d.\n", min, max);
    }
}

int leer_opcion_01(const char* mensaje) {
    char entrada[10];
    int valor;
    while (1) {
        printf("%s", mensaje);
        if (fgets(entrada, sizeof(entrada), stdin) != NULL) {
            if (sscanf(entrada, "%d", &valor) == 1 && (valor == 0 || valor == 1)) {
                return valor;
            }
        }
        printf("Entrada inválida. Ingresá 0 o 1.\n");
    }
}

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
    generar_laberinto_clasico(tablero);
    
    // Agregar muy pocos caminos extra (opcional)
    agregar_algunos_caminos_extra(tablero);
    
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
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    
    char** tablero_solucion = copiar_tablero(tablero);
    int exito = resolver_backtracking(tablero_solucion, 1, 1, modo_visual);
    
    QueryPerformanceCounter(&end);
    double tiempo = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    
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
    
    // Liberar memoria
    liberar_tablero(tablero);
    liberar_tablero(tablero_solucion);
    
    printf("\n╔══════════════════════════════════════╗\n");
    printf("║     ¡Gracias por usar Laberinto      ║\n");
    printf("║      Maestro! Presiona Enter...      ║\n");
    printf("╚══════════════════════════════════════╝\n");
    getchar();
    
    return 0;
}
