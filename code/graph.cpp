//
// Created by Madalena Ye on 11/03/2023.
//
#include "graph.h"


void Graph::addVertex(const int &id, Station station) {
    Vertex* v = new Vertex(id);
    v->setStation(station);
    vertexSet.push_back(v);
}
void Graph::addEdge(const int &sourc, const int &dest, int capacity, string service) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return;
    auto e1 = v1->addEdge(v2, capacity, service);
    auto e2 = v2->addEdge(v1, capacity, service);
    e1->setReverse(e2);
    e2->setReverse(e1);
}
vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}
Vertex * Graph::findVertex(const int &id) const {
    return vertexSet[id];
}
