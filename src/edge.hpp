#pragma once

template <typename T>
class Node;

/**
 * @brief - Clase que representa una arista del grafo
 */
template <typename T>
class Edge {
   private:
    Node<T>* m_destination;
    double m_weight;
    Edge<T>* m_next;

   public:
    Edge(Node<T>* destination) : m_destination(destination), m_weight(0.0), m_next(nullptr) {}

    Edge(Node<T>* destination, double weight) : m_destination(destination), m_weight(weight), m_next(nullptr) {}

    Node<T>* get_destination() const { return m_destination; }

    void set_destination(Node<T>* destination) { m_destination = destination; }

    double get_weight() const { return m_weight; }

    void set_weight(double weight) { m_weight = weight; }

    Edge<T>* get_next_edge() const { return m_next; }

    void set_next_edge(Edge<T>* next) { m_next = next; }
};
