#ifndef DA_GRAPH_H
#define DA_GRAPH_H

#include "vertexEdge.h"
#include "MutablePriorityQueue.h"
#include <algorithm>
#include <stack>

class Graph {
public:
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
    void addEdge(const int &source, const int &dest, int capacity, string service);

    std::vector<Vertex *> getVertexSet() const;
    int minCost(int source, int target);
    int maxFlow(int source, int target);

private:
    std::vector<Vertex *> vertexSet;    // vertex set
    bool findAugmentingPath(Vertex *src, Vertex *dest);

    bool findMinCostAugmentingPath(Vertex* src, Vertex* dest);

    int findMinResidualAlongPath(Vertex *src, Vertex *dest);

    void augmentFlowAlongPath(Vertex *src, Vertex *dest, int f);

    int pathCost(Vertex *src, Vertex *dest);
};
#endif //DA_GRAPH_H
