#include <iostream>
#include <string>

#include "graph_list.hpp"

int main() {
    GraphList<std::string> graph;
    graph.add_vertex("Madrid");
    graph.add_vertex("Barcelona");
    graph.add_edge("Madrid", "Barcelona", 621);
    std::cout << "Vertices: " << graph.get_vertex_count() << '\n';
    std::cout << "Distancia Madrid-Barcelona: " << graph.get_weight(0, 1) << '\n';
    return 0;
}
