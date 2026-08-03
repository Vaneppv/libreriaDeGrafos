#pragma once

#include <climits>
#include <iostream>
#include <limits>
#include <new>

#include "edge.hpp"
#include "node.hpp"

/**
 * @brief - Clase que representa un grafo no dirigido con lista de adyacencia
 */
template <typename T>
class GraphList {
private:
    Node<T>** m_vertices;
    int m_vertex_count;
    int m_capacity;

    bool grow();

    Edge<T>* find_edge(Node<T>* source, Node<T>* destination) const;

    bool remove_edge_between(Node<T>* source, Node<T>* destination);

public:
    GraphList() : m_vertices(nullptr), m_vertex_count(0), m_capacity(0) {}

    GraphList(const GraphList&) = delete;

    GraphList& operator=(const GraphList&) = delete;

    ~GraphList();

    int add_vertex(T data);

    int find_vertex(T data) const;

    T* get_vertex(int index) const;

    int get_vertex_count() const;

    bool add_edge(T from, T to, double weight);

    bool remove_edge(T from, T to);

    bool remove_vertex(T data);

    double get_weight(int from, int to) const;

    int bfs(T start, T target, T* path) const;

    bool dfs(T start, T target) const;

    double shortest_path(T from, T to, T* path) const;

    int count_components() const;
};

template <typename T>
GraphList<T>::~GraphList() {
    for (int i = 0; i < m_vertex_count; i++) {
        Edge<T>* edge = m_vertices[i]->get_edges();
        while (edge != nullptr) {
            Edge<T>* next = edge->get_next_edge();
            delete edge;
            edge = next;
        }
        delete m_vertices[i];
    }
    delete[] m_vertices;
}

template <typename T>
bool GraphList<T>::grow() {
    if (m_capacity > INT_MAX / 2) {
        std::cerr << "Error: overflow al agrandar el grafo" << std::endl;
        return false;
    }
    int new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
    Node<T>** new_vertices = nullptr;
    try {
        new_vertices = new Node<T>*[new_capacity];
    } catch (const std::bad_alloc&) {
        std::cerr << "Error: memoria insuficiente" << std::endl;
        return false;
    }
    for (int i = 0; i < m_vertex_count; i++) {
        new_vertices[i] = m_vertices[i];
    }
    delete[] m_vertices;
    m_vertices = new_vertices;
    m_capacity = new_capacity;
    return true;
}

template <typename T>
int GraphList<T>::add_vertex(T data) {
    if (find_vertex(data) != -1) {
        std::cerr << "Error: el vertice ya existe" << std::endl;
        return -1;
    }
    if (m_vertex_count == m_capacity && !grow()) {
        return -1;
    }
    try {
        m_vertices[m_vertex_count] = new Node<T>(data);
    } catch (const std::bad_alloc&) {
        std::cerr << "Error: memoria insuficiente" << std::endl;
        return -1;
    }
    return m_vertex_count++;
}

template <typename T>
int GraphList<T>::find_vertex(T data) const {
    for (int i = 0; i < m_vertex_count; i++) {
        if (m_vertices[i]->get_data() == data) {
            return i;
        }
    }
    return -1;
}

template <typename T>
T* GraphList<T>::get_vertex(int index) const {
    if (index < 0 || index >= m_vertex_count) {
        std::cerr << "Error: indice fuera de rango" << std::endl;
        return nullptr;
    }
    return m_vertices[index]->get_data_pointer();
}

template <typename T>
int GraphList<T>::get_vertex_count() const {
    return m_vertex_count;
}

template <typename T>
Edge<T>* GraphList<T>::find_edge(Node<T>* source, Node<T>* destination) const {
    Edge<T>* current = source->get_edges();
    while (current != nullptr) {
        if (current->get_destination() == destination) {
            return current;
        }
        current = current->get_next_edge();
    }
    return nullptr;
}

template <typename T>
bool GraphList<T>::add_edge(T from, T to, double weight) {
    int from_index = find_vertex(from);
    int to_index = find_vertex(to);
    if (from_index == -1 || to_index == -1) {
        std::cerr << "Error: vertice inexistente" << std::endl;
        return false;
    }
    if (from_index == to_index) {
        std::cerr << "Error: no se permiten aristas a si mismo" << std::endl;
        return false;
    }
    if (!(weight > 0.0)) {
        std::cerr << "Error: el peso debe ser positivo" << std::endl;
        return false;
    }
    if (find_edge(m_vertices[from_index], m_vertices[to_index]) != nullptr) {
        std::cerr << "Error: la arista ya existe" << std::endl;
        return false;
    }
    Edge<T>* forward = nullptr;
    Edge<T>* backward = nullptr;
    try {
        forward = new Edge<T>(m_vertices[to_index], weight);
        backward = new Edge<T>(m_vertices[from_index], weight);
    } catch (const std::bad_alloc&) {
        delete forward;
        std::cerr << "Error: memoria insuficiente" << std::endl;
        return false;
    }
    forward->set_next_edge(m_vertices[from_index]->get_edges());
    m_vertices[from_index]->set_edges(forward);
    backward->set_next_edge(m_vertices[to_index]->get_edges());
    m_vertices[to_index]->set_edges(backward);
    return true;
}

template <typename T>
bool GraphList<T>::remove_edge_between(Node<T>* source, Node<T>* destination) {
    Edge<T>* current = source->get_edges();
    Edge<T>* previous = nullptr;
    while (current != nullptr) {
        if (current->get_destination() == destination) {
            if (previous == nullptr) {
                source->set_edges(current->get_next_edge());
            } else {
                previous->set_next_edge(current->get_next_edge());
            }
            delete current;
            return true;
        }
        previous = current;
        current = current->get_next_edge();
    }
    return false;
}

template <typename T>
bool GraphList<T>::remove_edge(T from, T to) {
    int from_index = find_vertex(from);
    int to_index = find_vertex(to);
    if (from_index == -1 || to_index == -1) {
        std::cerr << "Error: vertice inexistente" << std::endl;
        return false;
    }
    if (!remove_edge_between(m_vertices[from_index], m_vertices[to_index])) {
        std::cerr << "Error: la arista no existe" << std::endl;
        return false;
    }
    remove_edge_between(m_vertices[to_index], m_vertices[from_index]);
    return true;
}

template <typename T>
bool GraphList<T>::remove_vertex(T data) {
    int index = find_vertex(data);
    if (index == -1) {
        std::cerr << "Error: vertice inexistente" << std::endl;
        return false;
    }
    Node<T>* removed = m_vertices[index];
    for (int i = 0; i < m_vertex_count; i++) {
        if (i != index) {
            remove_edge_between(m_vertices[i], removed);
        }
    }
    Edge<T>* edge = removed->get_edges();
    while (edge != nullptr) {
        Edge<T>* next = edge->get_next_edge();
        delete edge;
        edge = next;
    }
    delete removed;
    for (int i = index; i < m_vertex_count - 1; i++) {
        m_vertices[i] = m_vertices[i + 1];
    }
    m_vertex_count--;
    return true;
}

template <typename T>
double GraphList<T>::get_weight(int from, int to) const {
    if (from < 0 || from >= m_vertex_count || to < 0 || to >= m_vertex_count) {
        std::cerr << "Error: indice fuera de rango" << std::endl;
        return -1.0;
    }
    Edge<T>* edge = find_edge(m_vertices[from], m_vertices[to]);
    if (edge == nullptr) {
        return -1.0;
    }
    return edge->get_weight();
}

template <typename T>
int GraphList<T>::bfs(T start, T target, T* path) const {
    int start_index = find_vertex(start);
    int target_index = find_vertex(target);
    if (start_index == -1 || target_index == -1) {
        std::cerr << "Error: vertice inexistente" << std::endl;
        return -1;
    }
    if (start_index == target_index) {
        path[0] = start;
        return 0;
    }
    bool* visited = nullptr;
    int* predecessor = nullptr;
    int* queue = nullptr;
    try {
        visited = new bool[m_vertex_count];
        predecessor = new int[m_vertex_count];
        queue = new int[m_vertex_count];
    } catch (const std::bad_alloc&) {
        delete[] visited;
        delete[] predecessor;
        delete[] queue;
        std::cerr << "Error: memoria insuficiente" << std::endl;
        return -1;
    }
    for (int i = 0; i < m_vertex_count; i++) {
        visited[i] = false;
        predecessor[i] = -1;
    }
    int front = 0;
    int rear = 0;
    visited[start_index] = true;
    queue[rear++] = start_index;
    bool found = false;
    while (front < rear && !found) {
        int current = queue[front++];
        Edge<T>* edge = m_vertices[current]->get_edges();
        while (edge != nullptr) {
            int neighbor = find_vertex(edge->get_destination()->get_data());
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                predecessor[neighbor] = current;
                queue[rear++] = neighbor;
                if (neighbor == target_index) {
                    found = true;
                    break;
                }
            }
            edge = edge->get_next_edge();
        }
    }
    if (!found) {
        delete[] visited;
        delete[] predecessor;
        delete[] queue;
        return -1;
    }
    int count = 0;
    int current = target_index;
    while (current != -1) {
        queue[count++] = current;
        current = predecessor[current];
    }
    for (int i = 0; i < count; i++) {
        path[i] = *get_vertex(queue[count - 1 - i]);
    }
    int distance = count - 1;
    delete[] visited;
    delete[] predecessor;
    delete[] queue;
    return distance;
}

template <typename T>
bool GraphList<T>::dfs(T start, T target) const {
    int start_index = find_vertex(start);
    int target_index = find_vertex(target);
    if (start_index == -1 || target_index == -1) {
        std::cerr << "Error: vertice inexistente" << std::endl;
        return false;
    }
    bool* visited = nullptr;
    int* stack = nullptr;
    try {
        visited = new bool[m_vertex_count];
        stack = new int[m_vertex_count];
    } catch (const std::bad_alloc&) {
        delete[] visited;
        delete[] stack;
        std::cerr << "Error: memoria insuficiente" << std::endl;
        return false;
    }
    for (int i = 0; i < m_vertex_count; i++) {
        visited[i] = false;
    }
    int top = 0;
    stack[top++] = start_index;
    visited[start_index] = true;
    bool found = false;
    while (top > 0) {
        int current = stack[--top];
        std::cout << *get_vertex(current) << '\n';
        if (current == target_index) {
            found = true;
            break;
        }
        Edge<T>* edge = m_vertices[current]->get_edges();
        while (edge != nullptr) {
            int neighbor = find_vertex(edge->get_destination()->get_data());
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                stack[top++] = neighbor;
            }
            edge = edge->get_next_edge();
        }
    }
    delete[] visited;
    delete[] stack;
    if (!found) {
        std::cout << "Objetivo no encontrado" << '\n';
    }
    return found;
}

template <typename T>
double GraphList<T>::shortest_path(T from, T to, T* path) const {
    int from_index = find_vertex(from);
    int to_index = find_vertex(to);
    if (from_index == -1 || to_index == -1) {
        std::cerr << "Error: vertice inexistente" << std::endl;
        return -1.0;
    }
    if (from_index == to_index) {
        path[0] = from;
        return 0.0;
    }
    const double INF = std::numeric_limits<double>::max();
    double* dist = nullptr;
    int* predecessor = nullptr;
    bool* visited = nullptr;
    try {
        dist = new double[m_vertex_count];
        predecessor = new int[m_vertex_count];
        visited = new bool[m_vertex_count];
    } catch (const std::bad_alloc&) {
        delete[] dist;
        delete[] predecessor;
        delete[] visited;
        std::cerr << "Error: memoria insuficiente" << std::endl;
        return -1.0;
    }
    for (int i = 0; i < m_vertex_count; i++) {
        dist[i] = INF;
        predecessor[i] = -1;
        visited[i] = false;
    }
    dist[from_index] = 0.0;
    for (int step = 0; step < m_vertex_count; step++) {
        int current = -1;
        double best = INF;
        for (int i = 0; i < m_vertex_count; i++) {
            if (!visited[i] && dist[i] < best) {
                best = dist[i];
                current = i;
            }
        }
        if (current == -1) {
            break;
        }
        visited[current] = true;
        if (current == to_index) {
            break;
        }
        Edge<T>* edge = m_vertices[current]->get_edges();
        while (edge != nullptr) {
            int neighbor = find_vertex(edge->get_destination()->get_data());
            if (!visited[neighbor] && dist[current] < INF &&
                dist[current] + edge->get_weight() < dist[neighbor]) {
                dist[neighbor] = dist[current] + edge->get_weight();
                predecessor[neighbor] = current;
            }
            edge = edge->get_next_edge();
        }
    }
    if (dist[to_index] == INF) {
        delete[] dist;
        delete[] predecessor;
        delete[] visited;
        return -1.0;
    }
    int count = 0;
    int current = to_index;
    while (current != -1) {
        count++;
        current = predecessor[current];
    }
    current = to_index;
    for (int i = count - 1; i >= 0; i--) {
        path[i] = *get_vertex(current);
        current = predecessor[current];
    }
    double distance = dist[to_index];
    delete[] dist;
    delete[] predecessor;
    delete[] visited;
    return distance;
}

template <typename T>
int GraphList<T>::count_components() const {
    if (m_vertex_count == 0) {
        return 0;
    }
    bool* visited = nullptr;
    int* queue = nullptr;
    try {
        visited = new bool[m_vertex_count];
        queue = new int[m_vertex_count];
    } catch (const std::bad_alloc&) {
        delete[] visited;
        delete[] queue;
        std::cerr << "Error: memoria insuficiente" << std::endl;
        return -1;
    }
    for (int i = 0; i < m_vertex_count; i++) {
        visited[i] = false;
    }
    int components = 0;
    for (int i = 0; i < m_vertex_count; i++) {
        if (visited[i]) {
            continue;
        }
        components++;
        int front = 0;
        int rear = 0;
        visited[i] = true;
        queue[rear++] = i;
        while (front < rear) {
            int current = queue[front++];
            Edge<T>* edge = m_vertices[current]->get_edges();
            while (edge != nullptr) {
                int neighbor = find_vertex(edge->get_destination()->get_data());
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    queue[rear++] = neighbor;
                }
                edge = edge->get_next_edge();
            }
        }
    }
    delete[] visited;
    delete[] queue;
    return components;
}
