#ifndef DA_GRAPH_H
#define DA_GRAPH_H

#include "vertexEdge.h"

class Graph {
public:
    explicit Graph(int size);
    /*
    * Auxiliary function to find a vertex with a given ID.
    */
    Vertex *findVertex(const int &id) const;
    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    void addVertex(const int &id,Station station);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    void addEdge(const int &sourc, const int &dest, int capacity, string service);

    int getNumVertex() const;
    std::vector<Vertex *> getVertexSet() const;
protected:
    std::vector<Vertex *> vertexSet;    // vertex set

    int size;
};
#endif //DA_GRAPH_H
