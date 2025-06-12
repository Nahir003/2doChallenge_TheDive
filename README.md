# El Laberinto Maestro 

## ¿De qué trata este proyecto?
Generar un laberinto completamente aleatorio pero siempre resoluble, y además diseñar un algoritmo que lo resuelva automáticamente.
No se trataba solo de hacer un mapa sino de crear un mundo lógico donde cada paso tenga sentido, y donde el código no solo construya, sino que también encuentre la salida.
Es un codigo que fue programado en C, con visualización animada en consola, exploración manual y resolución automática.

## ¿Cómo generé el laberinto?
Utilicé una versión del algoritmo DFS (Depth-First Search) pero en forma iterativa con una pila manual. Solo se recorren celdas impares, y se abren caminos conectando vecinos saltando de dos en dos. Así me aseguro que siempre haya paredes entre caminos y se mantenga la estructura tipo "pared-camino-pared".
También agregué una función opcional que mete unos cuantos caminos extra, para que el laberinto no sea tan lineal y tenga más posibilidades sin romper la solvencia.

## Resolución automática con backtracking
Para resolver el laberinto, programé un algoritmo de Backtracking clásico, que arranca desde la entrada y prueba todos los caminos hasta encontrar la salida. Si activás el modo visual, podés ver cómo se va moviendo paso a paso. Cuando encuentra el camino, lo deja marcado para visualizarlo.
Además, medí el tiempo de resolución usando QueryPerformanceCounter para tener precisión en milisegundos.

## Modos que incluí
- Exploración manual: Podés moverte con WASD dentro del laberinto como jugador.
- Visualización paso a paso: Podés activar una animación que muestra cómo se resuelve.
- Mapa visual con íconos:
🟪	Pared
🔲	Camino
⚫	Entrada
🏁	Salida
👤	Jugador (modo manual)
🔳	Ruta de solución

## Cosas que aprendí y descubrí
La generación tipo DFS funciona muy bien para garantizar resolución, pero es muy lineal, así que tuve que meter caminos extra con cierta probabilidad para que se sienta más natural.
Agregar un visual paso a paso ayuda mucho a depurar y entender cómo piensa el algoritmo.
Usar emojis monoespaciados hace que la interfaz en consola sea más linda y clara.
Medir tiempo en C con QueryPerformanceCounter me dio una métrica real del rendimiento.

## Cosas que podría mejorar
Agregar modos más avanzados de generación (modo con un solo camino vs. múltiples).
Incluir obstáculos especiales: trampas, portales, puertas.
Pasar todo a C++ y modularizar con clases.
Agregar una IA que juegue sola en modo exploración (como el ratón del reto anterior).

## Requisitos técnicos
Compilador C (lo probé en Windows con gcc y Visual Studio).
Requiere windows.h solo para el Sleep() y el contador de rendimiento.
(Se puede adaptar fácilmente para Linux cambiando eso por usleep() y gettimeofday().)

## ¿Por qué hice esto?
Porque me encanta resolver problemas con lógica y hacer que el código cobre vida en pantalla.
Este proyecto fue una excusa perfecta para practicar estructuras dinámicas, recursividad, backtracking, generación procedural, interacción con el usuario.
