//
// Created by Madalena Ye on 11/03/2023.
//
#include "graph.h"


void Graph::addVertex(const int &id, Station station) {
    Vertex* v = new Vertex(id);
    v->setStation(station);
    vertexSet.push_back(v);
}
void Graph::addEdge(const int &source, const int &dest, int capacity, string service) {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return;
    auto e1 = v1->addEdge(v2, capacity/2, service);
    auto e2 = v2->addEdge(v1, capacity/2, service);
    e1->setReverse(e2);
    e2->setReverse(e1);
}
vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}
Vertex * Graph::findVertex(const int &id) const {
    return vertexSet[id];
}


void Graph::testAndVisit(std::queue<Vertex*>& queue, Edge* e, Vertex* w, int residual){
    if (!w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        queue.push(w);
    }
}


bool Graph::findAugmentingPath(Vertex* src, Vertex* dest){
    for (Vertex*  v : vertexSet)
        v->setVisited(false);

    src->setVisited(true);

    std::queue<Vertex*> queue;
    queue.push(src);

    while (!queue.empty() && !dest->isVisited()){
        Vertex* v = queue.front();
        queue.pop();

        for (Edge* e: v->getAdj())
            testAndVisit(queue,e,e->getDest(),e->getCapacity()-e->getFlow());

        for (Edge* e: v->getIncoming())
            testAndVisit(queue,e,e->getOrig(),e->getFlow());
    }

    return dest->isVisited();
}

int Graph::findMinResidualAlongPath(Vertex* src, Vertex* dest){
    int f = INF;
    for(Vertex* v = dest; v != src;){
        Edge* e = v->getPath();
        if (e->getDest() == v){
            f = std::min(f,e->getCapacity()-e->getFlow());
            v = e->getOrig();
        }
        else{
            f = std::min(f,e->getFlow());
            v = e->getDest();
        }
    }
    return f;
}

void Graph::augmentFlowAlongPath(Vertex *src, Vertex *dest, int f){
    for (Vertex* v = dest; v != src;){
        Edge* e = v->getPath();
        int flow = e->getFlow();

        //capacidade residual
        if (e->getDest() == v){
            e->setFlow(flow+f);
            v = e->getOrig();
        }

            //fluxo no sentido oposto
        else{
            e->setFlow(flow-f);
            v = e->getDest();
        }
    }
}


void Graph::edmondsKarp(int source, int target) {
    Vertex* src = findVertex(source);
    Vertex*  dest = findVertex(target);
    if (src == nullptr || dest == nullptr || src == dest)
        return;

    //reset dos fluxos
    for (Vertex*  v : vertexSet)
        for (Edge* e : v->getAdj())
            e->setFlow(0);

    //encontrar caminhos de aumento de fluxo
    while (findAugmentingPath(src,dest)) {
        auto f = findMinResidualAlongPath(src, dest);
        augmentFlowAlongPath(src, dest, f);
    }
}

int Graph::maxFlow(int source, int target){

    Vertex* src = findVertex(source);
    Vertex*  dest = findVertex(target);
    if (src == nullptr || dest == nullptr || src == dest)
        return -1;

    int maxFlow = 0;

    edmondsKarp(source, target);

    for(auto e : src->getAdj()) {
        maxFlow += e->getFlow();
    }
    return maxFlow;
}

int Graph::minCost(int source, int target) {
    Vertex* src = findVertex(source);
    Vertex* dest = findVertex(target);
    if (src == nullptr || dest == nullptr || src == dest)
        return -1;

    int cost = 0;

    edmondsKarp(source, target);

    for (int i = 1; i < vertexSet.size(); i++) {
        Vertex* v = vertexSet[i];
        for (auto e: v->getAdj()) {
            if (e->getFlow() != 0) {
                cost += 2;
                if (e->getService() == "STANDARD")
                    cost +=2;
                else
                    cost += 4;
            }
        }
    }
    return cost;
}