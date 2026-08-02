#pragma once

template <typename T>
class Edge;

/**
 * @brief - Clase que representa un vertice del grafo
 */
template <typename T>
class Node {
   private:
    T m_data;
    Edge<T>* m_edges;

   public:
    Node(T data) : m_data(data), m_edges(nullptr) {}

    T get_data() const { return m_data; }

    void set_data(T data) { m_data = data; }

    Edge<T>* get_edges() const { return m_edges; }

    void set_edges(Edge<T>* edges) { m_edges = edges; }
};
