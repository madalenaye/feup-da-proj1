#ifndef DA_VERTEXEDGE_H
#define DA_VERTEXEDGE_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

#include "Station.h"

class Edge;

#define INF std::numeric_limits<int>::max()

class Vertex {
public:
    Vertex(int id);

    bool operator<(Vertex& vertex) const;

    int getId() const;
    std::vector<Edge *> getAdj() const;

    int getPathCost() const;
    Edge *getPath() const;
    Station getStation() const;

    void setStation(Station station);
    void setVisited(bool visited);
    void setPathCost(int pathCost);
    void setPath(Edge *path);

    bool isVisited() const;
    Edge * addEdge(Vertex *dest, int capacity, const std::string& service);

    int queueIndex = 0;

private:
    int id;
    std::vector<Edge *> adj;
    std::vector<Edge *> incoming;
    bool visited = false;
    int pathCost = 0;
    Edge *path = nullptr;
    Station station = Station("");
};

/********************** Edge  ****************************/

class Edge {
public:
    Edge(Vertex *orig, Vertex *dest, int capacity, const std::string& service);

    Vertex * getDest() const;
    int getCapacity() const;
    int getResidualCapacity() const;

    Vertex * getOrig() const;
    Edge *getReverse() const;

    int getCost() const;

    void setReverse(Edge *reverse);
    void setResidualCapacity(int residualCapacity);

private:
    Vertex * dest;
    Vertex *orig;
    int capacity;
    int residualCapacity;
    int cost;
    std::string service;
    Edge *reverse = nullptr;
};

#endif //DA_VERTEXEDGE_H
