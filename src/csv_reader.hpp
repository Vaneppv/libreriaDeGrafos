#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/**
 * @brief - Carga los datos de un fichero CSV en el grafo
 * @tparam Graph Tipo de grafo (GraphList o GraphMatrix)
 * @param graph Referencia al grafo que se llena
 * @param path Ruta del fichero CSV (ciudad_a,ciudad_b,distancia)
 * @return true si el fichero se abrio correctamente
 */
template <typename Graph>
bool load_graph_from_csv(Graph& graph, const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo " << path << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        std::istringstream line_stream(line);
        std::string from;
        std::string to;
        double weight = 0.0;
        if (!std::getline(line_stream, from, ',') ||
            !std::getline(line_stream, to, ',') ||
            !(line_stream >> weight)) {
            std::cerr << "Error: linea invalida ignorada: " << line << std::endl;
            continue;
        }
        if (!(weight > 0.0)) {
            std::cerr << "Error: peso no valido ignorado: " << line << std::endl;
            continue;
        }
        if (graph.find_vertex(from) == -1) {
            graph.add_vertex(from);
        }
        if (graph.find_vertex(to) == -1) {
            graph.add_vertex(to);
        }
        graph.add_edge(from, to, weight);
    }
    return true;
}