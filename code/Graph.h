#ifndef DA_GRAPH_H
#define DA_GRAPH_H

#include <algorithm>
#include <stack>
#include <list>

#include "VertexEdge.h"
#include "PriorityQueue.h"

class Graph {
public:

    Vertex *findVertex(const int &id) const;
    void addVertex(const int &id,Station station);
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
