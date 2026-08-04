#pragma once

#include <climits>
#include <iostream>
#include <limits>
#include <new>

/**
 * @brief - Clase que representa un grafo no dirigido con matriz de adyacencia
 */
template <typename T>
class GraphMatrix {
public:
    static constexpr double NO_EDGE = -1.0;

private:
    double** m_matrix;
    T* m_data;
    int m_vertex_count;
    int m_capacity;

    bool grow();

public:
    GraphMatrix() : m_matrix(nullptr), m_data(nullptr), m_vertex_count(0), m_capacity(0) {}

    GraphMatrix(const GraphMatrix&) = delete;

    GraphMatrix& operator=(const GraphMatrix&) = delete;

    ~GraphMatrix();

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

    void all_pairs_shortest_path(double* dist, int* next) const;

    int count_components() const;
};

template <typename T>
GraphMatrix<T>::~GraphMatrix() {
    for (int i = 0; i < m_capacity; i++) {
        delete[] m_matrix[i];
    }
    delete[] m_matrix;
    delete[] m_data;
}

template <typename T>
bool GraphMatrix<T>::grow() {
    if (m_capacity > INT_MAX / 2) {
        std::cerr << "Error: overflow al agrandar el grafo" << std::endl;
        return false;
    }
    int new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
    double** new_matrix = nullptr;
    T* new_data = nullptr;
    int allocated_rows = 0;
    try {
        new_matrix = new double*[new_capacity];
        for (allocated_rows = 0; allocated_rows < new_capacity; allocated_rows++) {
            new_matrix[allocated_rows] = new double[new_capacity];
            for (int j = 0; j < new_capacity; j++) {
                new_matrix[allocated_rows][j] = NO_EDGE;
            }
        }
        new_data = new T[new_capacity];
    } catch (const std::bad_alloc&) {
        for (int i = 0; i < allocated_rows; i++) {
            delete[] new_matrix[i];
        }
        delete[] new_matrix;
        std::cerr << "Error: memoria insuficiente" << std::endl;
        return false;
    }
    for (int i = 0; i < m_vertex_count; i++) {
        for (int j = 0; j < m_vertex_count; j++) {
            new_matrix[i][j] = m_matrix[i][j];
        }
        new_data[i] = m_data[i];
    }
    for (int i = 0; i < m_capacity; i++) {
        delete[] m_matrix[i];
    }
    delete[] m_matrix;
    delete[] m_data;
    m_matrix = new_matrix;
    m_data = new_data;
    m_capacity = new_capacity;
    return true;
}

template <typename T>
int GraphMatrix<T>::add_vertex(T data) {
    if (find_vertex(data) != -1) {
        std::cerr << "Error: el vertice ya existe" << std::endl;
        return -1;
    }
    if (m_vertex_count == m_capacity && !grow()) {
        return -1;
    }
    for (int j = 0; j < m_vertex_count; j++) {
        m_matrix[j][m_vertex_count] = NO_EDGE;
    }
    for (int j = 0; j <= m_vertex_count; j++) {
        m_matrix[m_vertex_count][j] = NO_EDGE;
    }
    m_data[m_vertex_count] = data;
    return m_vertex_count++;
}

template <typename T>
int GraphMatrix<T>::find_vertex(T data) const {
    for (int i = 0; i < m_vertex_count; i++) {
        if (m_data[i] == data) {
            return i;
        }
    }
    return -1;
}

template <typename T>
T* GraphMatrix<T>::get_vertex(int index) const {
    if (index < 0 || index >= m_vertex_count) {
        std::cerr << "Error: indice fuera de rango" << std::endl;
        return nullptr;
    }
    return &m_data[index];
}

template <typename T>
int GraphMatrix<T>::get_vertex_count() const {
    return m_vertex_count;
}

template <typename T>
bool GraphMatrix<T>::add_edge(T from, T to, double weight) {
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
    if (m_matrix[from_index][to_index] != NO_EDGE) {
        std::cerr << "Error: la arista ya existe" << std::endl;
        return false;
    }
    m_matrix[from_index][to_index] = weight;
    m_matrix[to_index][from_index] = weight;
    return true;
}

template <typename T>
bool GraphMatrix<T>::remove_edge(T from, T to) {
    int from_index = find_vertex(from);
    int to_index = find_vertex(to);
    if (from_index == -1 || to_index == -1) {
        std::cerr << "Error: vertice inexistente" << std::endl;
        return false;
    }
    if (m_matrix[from_index][to_index] == NO_EDGE) {
        std::cerr << "Error: la arista no existe" << std::endl;
        return false;
    }
    m_matrix[from_index][to_index] = NO_EDGE;
    m_matrix[to_index][from_index] = NO_EDGE;
    return true;
}

template <typename T>
bool GraphMatrix<T>::remove_vertex(T data) {
    int index = find_vertex(data);
    if (index == -1) {
        std::cerr << "Error: vertice inexistente" << std::endl;
        return false;
    }
    int n = m_vertex_count;
    double* fresh_row = nullptr;
    try {
        fresh_row = new double[m_capacity];
    } catch (const std::bad_alloc&) {
        std::cerr << "Error: memoria insuficiente" << std::endl;
        return false;
    }
    for (int j = 0; j < m_capacity; j++) {
        fresh_row[j] = NO_EDGE;
    }
    double* removed_row = m_matrix[index];
    for (int i = 0; i < n; i++) {
        if (i == index) {
            continue;
        }
        for (int j = index + 1; j < n; j++) {
            m_matrix[i][j - 1] = m_matrix[i][j];
        }
        m_matrix[i][n - 1] = NO_EDGE;
    }
    for (int i = index; i < n - 1; i++) {
        m_matrix[i] = m_matrix[i + 1];
    }
    m_matrix[n - 1] = fresh_row;
    delete[] removed_row;
    for (int i = index; i < n - 1; i++) {
        m_data[i] = m_data[i + 1];
    }
    m_vertex_count--;
    return true;
}

template <typename T>
double GraphMatrix<T>::get_weight(int from, int to) const {
    if (from < 0 || from >= m_vertex_count || to < 0 || to >= m_vertex_count) {
        std::cerr << "Error: indice fuera de rango" << std::endl;
        return NO_EDGE;
    }
    return m_matrix[from][to];
}

template <typename T>
int GraphMatrix<T>::bfs(T start, T target, T* path) const {
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
        for (int j = 0; j < m_vertex_count; j++) {
            if (m_matrix[current][j] != NO_EDGE && !visited[j]) {
                visited[j] = true;
                predecessor[j] = current;
                queue[rear++] = j;
                if (j == target_index) {
                    found = true;
                    break;
                }
            }
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
bool GraphMatrix<T>::dfs(T start, T target) const {
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
        for (int j = m_vertex_count - 1; j >= 0; j--) {
            if (m_matrix[current][j] != NO_EDGE && !visited[j]) {
                visited[j] = true;
                stack[top++] = j;
            }
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
double GraphMatrix<T>::shortest_path(T from, T to, T* path) const {
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
        for (int j = 0; j < m_vertex_count; j++) {
            if (m_matrix[current][j] != NO_EDGE && !visited[j] &&
                dist[current] < INF && dist[current] + m_matrix[current][j] < dist[j]) {
                dist[j] = dist[current] + m_matrix[current][j];
                predecessor[j] = current;
            }
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
