#ifndef DA_GRAPH_H
#define DA_GRAPH_H

#include <algorithm>
#include <stack>
#include <list>

#include "VertexEdge.h"
#include "PriorityQueue.h"

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
    void addEdge(const int &source, const int &dest, int capacity, const std::string& service) const;

    std::vector<Vertex *> getVertexSet() const;
    int minCost(int source, int target);
    int maxFlow(int source, int target);

    unsigned int maxConnectedDistrict(const std::string& district);
    unsigned int maxConnectedMunicipality(const std::string& municipality);


private:
    std::vector<Vertex *> vertexSet;

    bool findAugmentingPath(Vertex *src, Vertex *dest);

    bool findMinCostAugmentingPath(Vertex* src, Vertex* dest);

    static int findMinResidualAlongPath(Vertex *src, Vertex *dest);

    static void augmentFlowAlongPath(Vertex *src, Vertex *dest, int flow);

    static int pathCost(Vertex *src, Vertex *dest);

    void dfsConnectedDistrict(Vertex* v, std::list<int>& comp, const std::string& district);

    void dfsConnectedMunicipality(Vertex* v, std::list<int>& comp, const std::string& municipality);
};
#endif //DA_GRAPH_H
