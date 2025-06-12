#include <stdio.h> //entrada y salida estándar: printf,scanf etc.
#include <time.h> // necesario para time() para inicializar rand() con srand(time(NULL)).
#include <windows.h> // para Sleep()
#include <stdlib.h> /*para funciones de manejo de memoria dinámica como malloc(), calloc() y free() y generar números aleatorios con rand, srand.
                    Memory ALLOCation reservar un espacio de memoria dinámicamente en el heap (montón), en tiempo de ejecución.
                    free Sirve para liberar la memoria que reservaste con malloc.(Ya no necesito este espacio. Podés reciclarlo
                    calloc Reserva y pone todo en 0.)*/
#include <string.h> //Trae funciones como memcpy, strcpy, strcmp
#include "laberinto.h"

/* Un puntero es una variable que guarda una dirección de memoria se representa con *
 un puntero doble es un puntero que apunta a otro puntero se representa con **  */

/*Malloc= Permite crear matrices dinámicas y grandes sin reventar el stack.
Puede definir el tamaño del laberinto en tiempo de ejecución, retornarlo desde funciones y usar más memoria (heap), liberándola después con free().*/

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

/*1. Crear tablero lleno de muros con '#'
2. Crear matriz "visitado" inicializada en 0
3. Elegir celda de inicio (1,1)
4. Marcar inicio como visitado y camino '*'
5. Crear pila DFS y poner la celda inicial
6. Mientras haya celdas en la pila:
    7. Tomar la última celda (pop)
    8. Buscar sus vecinos válidos:
        - Deben estar dentro del laberinto
        - Ser impares (celdas reales)
        - No estar visitados
    9. Si tiene vecinos:
        10. Volver a meter la celda actual a la pila
        11. Elegir un vecino al azar
        12. Marcarlo como visitado y como camino '*'
        13. Abrir la pared intermedia entre ambos
        14. Agregar el vecino a la pila (push)
15. Cuando la pila está vacía, el laberinto está generado
16. Liberar memoria usada (stack y visitado)
*/

//stack es una estructura de datos que funciona con lifo (ultimo en entrar primero en salir )

void generar_laberinto(char** tablero) {
    //Esta función es iterativa, y su objetivo es crear caminos al azar para crear un laberinto

    // Stack para DFS iterativo se usa para recordar la trayectoria
    Coordenada* stack = (Coordenada*) malloc((filas * columnas) * sizeof(Coordenada));
    int stack_top = 0; /*Cada vez que visitás una celda, la guardás en stack 
                        cuando ya no hay más caminos desde ahí, hacés pop y volvés atrás
                        DFS tradicional usa pila recursiva, pero como no usamos recursión
                        Creamos una pila dinámica que guarda coordenadas */
    
    // Matriz de visitados para las celdas (solo posiciones impares)
    int** visitado = (int**) malloc(filas * sizeof(int*));
    for (int i = 0; i < filas; i++) { 
        visitado[i] = (int*) calloc(columnas, sizeof(int)); //Para cada fila, usamos calloc para crear una array de columnas.
                                                            // además inicializa todo en 0, así que todas las celdas comienzan como no visitadas.
    }
    
    // Comenzar desde (1,1) - primera celda válida
    int start_x = 1, start_y = 1;
    stack[stack_top++] = (Coordenada){start_x, start_y}; //Hacemos push de la coordenada inicial a la pila.
    visitado[start_x][start_y] = 1;  // Si visitado[x][y] == 1 → ya pasamos por esa celda.
    tablero[start_x][start_y] = '*'; // Marcar como camino
    
    // Direcciones: arriba, abajo, izquierda, derecha (saltos de 2)
    int dx[] = {-2, 2, 0, 0};
    int dy[] = {0, 0, -2, 2};
    
    // se hace while para no acceder a una posición inválida en memoria (stack[-1])
    //lo cual podría causar lectura basura, comportamiento inesperado o incluso un crash 
    while (stack_top > 0) {
        Coordenada actual = stack[--stack_top]; //Saco la última celda agregada a la pila.
                                                //nodo actual desde donde vas a buscar vecinos
        int x = actual.x, y = actual.y; //Separo sus coordenadas para usarlas más fácilmente.
        
        // Encontrar vecinos no visitados
        int vecinos_validos[4]; // posibles direcciones a tomar
        int num_vecinos = 0;    // cuantos vecinos libres encontre
        
        //Recorre las 4 direcciones 
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            // Verificar que esté dentro de los límites (no salirte del tablero, no visitar bordes)
            // Que no se repitan (ya visitadas) y solo elegir celdas impares (válidas).
            if (nx > 0 && ny > 0 && nx < filas - 1 && ny < columnas - 1 && 
                nx % 2 == 1 && ny % 2 == 1 && !visitado[nx][ny]) {
                vecinos_validos[num_vecinos++] = i;
            }
        }
        
        //si hay al menos un vecino libre
        if (num_vecinos > 0) {
            // Volver a poner la celda actual en la pila
            stack[stack_top++] = actual; //hay más caminos que explorar desde ahí luego.
            
            // Elegir un vecino aleatorio
            int dir_elegida = vecinos_validos[rand() % num_vecinos];
            int nx = x + dx[dir_elegida];
            int ny = y + dy[dir_elegida];
            
            // Marcar el vecino como visitado y como camino
            visitado[nx][ny] = 1;
            tablero[nx][ny] = '*';
            
            // Abrir la pared entre la celda actual y el vecino
            //se divide entre 2 porque querés caer justo en la celda intermedia, que es donde está el muro a romper.
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


int calcular_cobertura(char** tablero) {
    /* Mide cuántas celdas del tablero son camino (*, 'E', 'S') vs. total de celdas internas.
    te da un porcentaje de "cobertura" del laberinto: cuánto está tallado (camino) frente a lo que sigue siendo muro.*/
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

/*1. ¿Estoy fuera del tablero? → STOP
2. ¿Estoy en la salida? → ÉXITO
3. ¿Es camino o entrada? → OK
4. Marco '+'
5. Intento moverse en 4 direcciones recursivamente
6. Si ninguna sirve, desmarco y vuelvo atrás
*/

int resolver_backtracking(char** tablero, int x, int y, int modo_visual) {
    //Esta función es recursiva, y su objetivo es encontrar la salida ('S') a partir de una celda inicial (x, y).
    if (x < 0 || x >= filas || y < 0 || y >= columnas) return 0; //Verifica que (x, y) esté dentro del tablero.
    
    if (tablero[x][y] == 'S') return 1;//Si estás sobre la salida, termina
    
    if (tablero[x][y] != '*' && tablero[x][y] != 'E') return 0; //Solo podés avanzar por caminos ('*') o por la entrada ('E').
                                                                //Si estás en un muro ('#') o en un lugar ya visitado → volvés atrás.
    
    char original = tablero[x][y]; //original guarda el contenido previo de la celda actual para poder restaurarlo 
                                //si el camino no lleva a la salida.
    if (tablero[x][y] != 'E') tablero[x][y] = '+';
    
    if (modo_visual) {
        limpiar_pantalla();
        printf("Resolviendo paso a paso...\n");
        imprimir_tablero(tablero);
        Sleep(100);//100 milisegundos 
    }
    
    //se establece los 4 movimientos cardinales.
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    // Intenta moverse en cada dirección.
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        /*Y llama recursivamente a la función.
        Si alguna llamada retorna 1, es porque encontró la salida 
        Si todas fallan, retrocede...*/
        if (resolver_backtracking(tablero, nx, ny, modo_visual)) {
            return 1;
        }
    }
    // (backtracking):Si no hubo salida por acá, borra la marca y vuelve a como estaba.
    if (original != 'E') tablero[x][y] = original;
    return 0;
}

int se_puede_resolver(char** tablero, int x, int y) {
    //funcion recursiva tiene como objetivo saber si existe algún camino desde la entrada (E) 
    //hasta la salida (S) sin mostrar el recorrido ni modificar el laberinto visiblemente.
    //utilizo para corroborar al 100% que es un laberinto resolvible

    //Salida de límites → no hay camino posible.
    if (x < 0 || x >= filas || y < 0 || y >= columnas) return 0;
    if (tablero[x][y] == 'S') return 1;
    //Solo podemos caminar por '*' (caminos) o 'E' (entrada).
    if (tablero[x][y] != '*' && tablero[x][y] != 'E') return 0;
    
    char temp = tablero[x][y];
    tablero[x][y] = 'x'; // Marcar como visitado
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    //Intentás moverte en todas direcciones.
    //Si alguna rama lleva a la salida, restaurás el valor original y devolvés 1
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i], ny = y + dy[i];
        if (se_puede_resolver(tablero, nx, ny)) {
            tablero[x][y] = temp;
            return 1;
        }
    }
    
    //Si ninguna rama funcionó, devolvés la celda a su estado anterior ('*' o 'E') y decís "no hay salida por acá".
    tablero[x][y] = temp;
    return 0;
}

void explorar_manual(char** tablero) {
    int x = 1, y = 1; // Posición inicial del jugador
    char input;
    
    printf("\n=== EXPLORACIÓN MANUAL ===\n");
    printf("Controles: W(arriba) S(abajo) A(izquierda) D(derecha) Q(salir)\n");
    printf("Objetivo: Llegar a la bandera 🏁\n\n");
    
    do {
        limpiar_pantalla(); //Limpia la pantalla (como si "borrara" la anterior)
        printf("=== EXPLORACIÓN MANUAL ===\n");
        printf("Controles: WASD para mover, Q para salir\n");
        printf("Posición actual: (%d, %d)\n\n", x, y);
        
        char temp = tablero[x][y]; // Guardo lo que había
        tablero[x][y] = '-';       // Pongo el jugador
        imprimir_tablero(tablero);
        tablero[x][y] = temp;       // Vuelvo a dejarlo como estaba
        
        printf("\n Tu movimiento: ");
        scanf(" %c", &input); // Leer una tecla (w, a, s, d, q)
        
        //Según la tecla, cambiás nx o ny.
        int nx = x, ny = y;
        switch (input) {
            case 'w': case 'W': nx--; break;
            case 's': case 'S': nx++; break;
            case 'a': case 'A': ny--; break;
            case 'd': case 'D': ny++; break;
        }

        // Verificar que esté dentro de los límites (no salirte del tablero, no visitar bordes)
        // Que no se repitan (ya visitadas) y solo elegir celdas impares (válidas).
        if (nx >= 0 && ny >= 0 && nx < filas && ny < columnas &&
            (tablero[nx][ny] == '*' || tablero[nx][ny] == 'S')) {
            x = nx;
            y = ny;
        } else if (input != 'q' && input != 'Q') {
            printf("No podés moverte ahí. Presiona Enter...");
            while (getchar() != '\n');
            getchar(); //pausa el programa hasta que el usuario presione Enter
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
            while (getchar() != '\n'); // el programa se detiene aquí hasta que toques Enter
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

