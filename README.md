# Librería de Grafos

Librería header-only en C++20 para trabajar con grafos no dirigidos y con peso,
implementada con lista de adyacencia (`GraphList`) y con matriz de adyacencia
(`GraphMatrix`). Es "incluir y usar": basta con copiar el header y `#include`
desde cualquier proyecto.

## Estructura de carpetas

- `src/` — código de la librería y del programa
- `bin/` — ejecutable generado y datos de prueba (`datos.csv`)
- `build/` — objetos y `compile_commands.json` (generados por make)

## Requisitos

- Compilador con soporte C++20 (g++ 11+ o superior)
- make
- clang-format (opcional, solo para `make format-check`)

## Compilación y ejecución

    make
    ./bin/grafos

Otros targets:

- `make compile_commands` — genera `build/compile_commands.json` (para clangd)
- `make format-check` — verifica el formato con clang-format
- `make clean` — elimina binarios y `build/`

## Uso de la librería

```cpp
#include "graph_list.hpp"

GraphList<std::string> graph;
graph.add_vertex("Madrid");
graph.add_vertex("Barcelona");
graph.add_edge("Madrid", "Barcelona", 621);

int madrid = graph.find_vertex("Madrid");
double distancia = graph.get_weight(madrid, 1);
```

El grafo es no dirigido y con pesos positivos. La clase es plantilla: el dato
del vértice puede ser cualquier tipo con `operator==`. Las dos implementaciones
(`GraphList` y `GraphMatrix`) comparten la misma API, por lo que son
intercambiables sin cambiar el código que las usa.

## API actual

### GraphList<T>

| Método                        | Descripción                                            |
| ----------------------------- | ------------------------------------------------------ |
| `int add_vertex(T data)`      | Agrega un vértice; devuelve su índice o `-1` (duplicado) |
| `int find_vertex(T data)`     | Índice del vértice o `-1`                              |
| `T* get_vertex(int index)`    | Puntero al dato, `nullptr` si el índice es inválido    |
| `int get_vertex_count()`      | Cantidad de vértices                                   |
| `bool add_edge(T, T, double)` | Agrega arista no dirigida con peso                     |
| `bool remove_edge(T, T)`      | Elimina la arista                                      |
| `bool remove_vertex(T)`       | Elimina el vértice y sus aristas (reindexa)            |
| `double get_weight(int, int)` | Peso de la arista o `-1` si no existe                  |
| `int bfs(T, T, T*)`           | Ruta y distancia (aristas) entre dos vértices (`-1` si no hay) |
| `bool dfs(T, T)`              | Recorrido en profundidad; `false` si no encuentra el objetivo |
| `double shortest_path(T, T, T*)` | Camino más corto (Dijkstra) con peso total (`< 0` si no hay) |
| `int count_components()`      | Número de componentes conexas                          |

### GraphMatrix<T>

`GraphMatrix` ofrece la misma API que `GraphList` y además:

| Método                                     | Descripción                        |
| ------------------------------------------ | ---------------------------------- |
| `void all_pairs_shortest_path(double*, int*)` | Distancias entre todos los pares (Floyd-Warshall) |

Los fallos (índice fuera de rango, vértice inexistente, memoria insuficiente)
se reportan por `stderr` y con valor de retorno de error.

## Estado del proyecto

Implementado y probado en ambas implementaciones (`GraphList` y `GraphMatrix`):
gestión de vértices y aristas, verificación de límites y seguridad de memoria,
recorridos BFS y DFS, camino más corto (Dijkstra), distancias entre todos los
pares (Floyd-Warshall, en `GraphMatrix`), componentes conexas, lector de CSV y
menú interactivo persistente.

## Referencias

- `requirements.md` — enunciado del proyecto
- `styleguides.md` — convenciones de nombres, formato y commits
