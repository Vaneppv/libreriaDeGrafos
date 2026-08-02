#pragma once

#include <climits>
#include <iostream>

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

public:
    GraphList() : m_vertices(nullptr), m_vertex_count(0), m_capacity(0) {}

    ~GraphList();

    int add_vertex(T data);

    int find_vertex(T data) const;

    T* get_vertex(int index) const;

    int get_vertex_count() const;
};

template <typename T>
GraphList<T>::~GraphList() {
    for (int i = 0; i < m_vertex_count; i++) {
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
    Node<T>** new_vertices = new Node<T>*[new_capacity];
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
    m_vertices[m_vertex_count] = new Node<T>(data);
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
