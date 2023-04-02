//
// Created by Madalena Ye on 11/03/2023.
//

#ifndef DA_VERTEXEDGE_H
#define DA_VERTEXEDGE_H


#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "station.h"
using namespace std;

class Edge;

#define INF std::numeric_limits<int>::max()

class Vertex {
public:
    Vertex(int id);
    bool operator<(Vertex & vertex) const; // // required by MutablePriorityQueue

    int getId() const;
    std::vector<Edge *> getAdj() const;
    bool isVisited() const;
    bool isProcessing() const;
    unsigned int getIndegree() const;
    int getDist() const;
    Edge *getPath() const;
    std::vector<Edge *> getIncoming() const;
    Station getStation() const;

    void setId(int info);
    void setStation(Station station);

    void setVisited(bool visited);
    void setProcesssing(bool processing);
    void setIndegree(unsigned int indegree);
    void setDist(int dist);
    void setPath(Edge *path);
    Edge * addEdge(Vertex *dest, int capacity, string service);
    bool removeEdge(int destID);
    void removeOutgoingEdges();


    int queueIndex = 0;
private:
    int id;                // identifier
    std::vector<Edge *> adj;  // outgoing edges
    Station station = Station("");

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    unsigned int indegree; // used by topsort
    int dist = 0;
    Edge *path = nullptr;



    std::vector<Edge *> incoming; // incoming edges

    // required by MutablePriorityQueue and UFDS

    void deleteEdge(Edge *edge);
};

/********************** Edge  ****************************/

class Edge {
public:
    Edge(Vertex *orig, Vertex *dest, int capacity, string service);

    Vertex * getDest() const;
    int getCapacity() const;
    int getResidualCapacity() const;
    string getService() const;
    bool isSelected() const;
    Vertex * getOrig() const;
    Edge *getReverse() const;
    int getFlow() const;
    int getCost() const;
    void setSelected(bool selected);
    void setReverse(Edge *reverse);
    void setFlow(int flow);
    void setResidualCapacity(int residualCapacity);
private:
    Vertex * dest; // destination vertex
    int capacity; // edge weight, can also be used for capacity
    int residualCapacity;
    string service;
    // auxiliary fields
    bool selected = false;
    int cost;
    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    int flow; // for flow-related problems

    void setCost(int cost);
};

#endif //DA_VERTEXEDGE_H
