#pragma once

#include <iostream>
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

    ~GraphMatrix();

    int add_vertex(T data);

    int find_vertex(T data) const;

    T* get_vertex(int index) const;

    int get_vertex_count() const;

    bool add_edge(T from, T to, double weight);

    bool remove_edge(T from, T to);

    bool remove_vertex(T data);

    double get_weight(int from, int to) const;
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
    m_matrix[n - 1] = nullptr;
    delete[] removed_row;
    for (int i = index; i < n - 1; i++) {
        m_data[i] = m_data[i + 1];
    }
    m_vertex_count--;
    return true;
}

template <typename T>
double GraphMatrix<T>::get_weight(int from, int to) const {
    return m_matrix[from][to];
}
