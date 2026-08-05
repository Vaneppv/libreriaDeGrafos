#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "csv_reader.hpp"
#include "graph_list.hpp"
#include "graph_matrix.hpp"

namespace {
    const std::string DATA_FILE = "bin/datos.csv";

    void show_menu() {
        std::cout << '\n';
        std::cout << "=== Grafo de ciudades ===\n";
        std::cout << "1. Mostrar grafo\n";
        std::cout << "2. BFS entre dos ciudades\n";
        std::cout << "3. DFS entre dos ciudades\n";
        std::cout << "4. Camino mas corto (Dijkstra)\n";
        std::cout << "5. Floyd-Warshall: distancias entre todas las ciudades\n";
        std::cout << "6. Componentes conexas\n";
        std::cout << "0. Salir\n";
        std::cout << "Opcion: ";
    }

    template <typename Graph>
    std::string prompt_city(Graph& graph, const std::string& message) {
        std::cout << message;
        std::string city;
        std::getline(std::cin, city);
        if (graph.find_vertex(city) == -1) {
            std::cout << "Error: la ciudad \"" << city << "\" no existe"
                      << std::endl;
            return std::string();
        }
        return city;
    }

    void print_path(std::string* path, const std::string& target, int max_size) {
        for (int i = 0; i < max_size; i++) {
            if (i > 0) {
                std::cout << " -> ";
            }
            std::cout << path[i];
            if (path[i] == target) {
                return;
            }
        }
    }
}

int main() {
    GraphList<std::string> list_graph;
    GraphMatrix<std::string> matrix_graph;
    if (!load_graph_from_csv(list_graph, DATA_FILE)) {
        std::cerr << "Error: no se pudo cargar " << DATA_FILE << std::endl;
        return 1;
    }
    if (!load_graph_from_csv(matrix_graph, DATA_FILE)) {
        std::cerr << "Error: no se pudo cargar " << DATA_FILE << std::endl;
        return 1;
    }

    int option = 0;
    do {
        show_menu();
        if (!(std::cin >> option)) {
            if (std::cin.eof()) {
                break;
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cerr << "Error: opcion no valida" << std::endl;
            continue;
        }
        std::cin.ignore(10000, '\n');
        switch (option) {
            case 0:
                break;
            case 1: {
                int count = list_graph.get_vertex_count();
                std::cout << "Grafo con " << count << " vertices:\n";
                for (int i = 0; i < count; i++) {
                    std::cout << "  * " << *list_graph.get_vertex(i) << '\n';
                }
                std::cout << "Aristas con peso:\n";
                for (int i = 0; i < count; i++) {
                    for (int j = i + 1; j < count; j++) {
                        double weight = list_graph.get_weight(i, j);
                        if (weight > 0.0) {
                            std::cout << "  " << *list_graph.get_vertex(i)
                                      << " -- " << *list_graph.get_vertex(j)
                                      << " (" << weight << " km)\n";
                        }
                    }
                }
                break;
            }
            case 2: {
                std::string from = prompt_city(list_graph, "Ciudad de origen: ");
                if (from.empty()) {
                    break;
                }
                std::string to = prompt_city(list_graph, "Ciudad de destino: ");
                if (to.empty()) {
                    break;
                }
                int count = list_graph.get_vertex_count();
                std::string* path = new std::string[count];
                int distance = list_graph.bfs(from, to, path);
                if (distance < 0) {
                    std::cout << "No hay ruta de " << from << " a " << to
                              << '\n';
                } else {
                    std::cout << "BFS " << from << " -> " << to << " | ruta: ";
                    print_path(path, to, count);
                    std::cout << " | distancia: " << distance << '\n';
                }
                delete[] path;
                break;
            }
            case 3: {
                std::string from = prompt_city(list_graph, "Ciudad de origen: ");
                if (from.empty()) {
                    break;
                }
                std::string to = prompt_city(list_graph, "Ciudad de destino: ");
                if (to.empty()) {
                    break;
                }
                std::cout << "DFS " << from << " -> " << to << ":\n";
                list_graph.dfs(from, to);
                break;
            }
            case 4: {
                std::string from = prompt_city(list_graph, "Ciudad de origen: ");
                if (from.empty()) {
                    break;
                }
                std::string to = prompt_city(list_graph, "Ciudad de destino: ");
                if (to.empty()) {
                    break;
                }
                int count = list_graph.get_vertex_count();
                std::string* list_path = new std::string[count];
                std::string* matrix_path = new std::string[count];
                double list_distance = list_graph.shortest_path(from, to, list_path);
                double matrix_distance = matrix_graph.shortest_path(from, to, matrix_path);
                std::cout << "Dijkstra (lista): ";
                if (list_distance < 0.0) {
                    std::cout << "sin ruta\n";
                } else {
                    std::cout << list_distance << " km | ruta: ";
                    print_path(list_path, to, count);
                    std::cout << '\n';
                }
                std::cout << "Dijkstra (matriz): ";
                if (matrix_distance < 0.0) {
                    std::cout << "sin ruta\n";
                } else {
                    std::cout << matrix_distance << " km | ruta: ";
                    print_path(matrix_path, to, count);
                    std::cout << '\n';
                }
                bool same = (list_distance >= 0.0 && matrix_distance >= 0.0 &&
                             list_distance == matrix_distance);
                std::cout << "Ambas implementaciones coinciden: "
                          << (same ? "si" : "no") << '\n';
                delete[] list_path;
                delete[] matrix_path;
                break;
            }
            case 5: {
                int count = matrix_graph.get_vertex_count();
                double* dist = new double[count * count];
                int* next = new int[count * count];
                matrix_graph.all_pairs_shortest_path(dist, next);
                const double INF = std::numeric_limits<double>::max();
                int width = 0;
                for (int i = 0; i < count; i++) {
                    int len = static_cast<int>(matrix_graph.get_vertex(i)->length());
                    if (len > width) {
                        width = len;
                    }
                }
                std::cout << "Floyd-Warshall: distancias (km)\n";
                std::cout << std::setw(width) << "" << " |";
                for (int j = 0; j < count; j++) {
                    std::cout << " " << std::setw(width)
                              << *matrix_graph.get_vertex(j);
                }
                std::cout << '\n';
                for (int i = 0; i < count; i++) {
                    std::cout << std::setw(width) << *matrix_graph.get_vertex(i)
                              << " |";
                    for (int j = 0; j < count; j++) {
                        double value = dist[(i * count) + j];
                        if (i == j || value >= INF) {
                            std::cout << " " << std::setw(width) << "-";
                        } else {
                            std::cout << " " << std::setw(width) << value;
                        }
                    }
                    std::cout << '\n';
                }
                delete[] dist;
                delete[] next;
                break;
            }
            case 6: {
                int list_components = list_graph.count_components();
                int matrix_components = matrix_graph.count_components();
                std::cout << "Componentes conexas (lista): " << list_components
                          << '\n';
                std::cout << "Componentes conexas (matriz): " << matrix_components
                          << '\n';
                break;
            }
            default:
                std::cerr << "Error: opcion no valida" << std::endl;
                break;
        }
    } while (option != 0);

    return 0;
}