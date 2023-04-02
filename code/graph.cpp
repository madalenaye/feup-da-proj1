#include "graph.h"
#include "MutablePriorityQueue.h"


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

bool Graph::findAugmentingPath(Vertex* src, Vertex* dest){
    for (Vertex*  v : vertexSet)
        v->setVisited(false);

    src->setVisited(true);

    std::queue<Vertex*> queue;
    queue.push(src);

    while (!queue.empty() && !dest->isVisited()){
        Vertex* v = queue.front();
        queue.pop();

        for (Edge* e: v->getAdj()) {
            Vertex* w = e->getDest();
            int residual = e->getResidualCapacity();
            if (!w->isVisited() && residual > 0) {
                w->setVisited(true);
                w->setPath(e);
                queue.push(w);
            }
        }
    }

    return dest->isVisited();
}

int Graph::findMinResidualAlongPath(Vertex* src, Vertex* dest){
    int f = INF;
    for(Vertex* v = dest; v != src;){
        Edge* e = v->getPath();
        f = std::min(f,e->getResidualCapacity());
        v = e->getOrig();
    }
    return f;
}
void Graph::augmentFlowAlongPath(Vertex *src, Vertex *dest, int f){
    for (Vertex* v = dest; v != src;){
        Edge* e = v->getPath();
        Edge* reverse = e->getReverse();

        e->setResidualCapacity(e->getResidualCapacity()-f);
        reverse->setResidualCapacity(reverse->getResidualCapacity()+f);

        v = e->getOrig();
    }
}

void printPath(Vertex* src,Vertex* dest){
    stack<Vertex*> r;
    for (Vertex* v = dest; v != src;){
        r.push(v);
        Edge* e = v->getPath();
        if (e->getDest() == v){
            v = e->getOrig();
        }

        else{
            v = e->getDest();
        }
    }
    cout << src->getStation().getName() << " - ";
    while (!r.empty()){
        cout << r.top()->getStation().getName() << " - ";
        r.pop();
    }
    cout << endl;
}
int Graph::maxFlow(int source, int target){

    Vertex* src = findVertex(source);
    Vertex* dest = findVertex(target);
    if (src == nullptr || dest == nullptr || src == dest)
        return 0;

    int flow = 0;

    for (Vertex* vertex : vertexSet)
        for (Edge* edge : vertex->getAdj())
            edge->setResidualCapacity(edge->getCapacity());

    int i = 1;

    while (findAugmentingPath(src,dest)) {
        auto f = findMinResidualAlongPath(src, dest);
        augmentFlowAlongPath(src, dest, f);
        cout << "Path nr. " << i++ << " : ";
        printPath(src,dest);
        cout << endl;
        flow+=f;
    }
    return flow;
}

bool Graph::findMinCostAugmentingPath(Vertex* src, Vertex* dest){
    for(Vertex* v: vertexSet){
        v->setDist(INF);
        v->setPath(nullptr);
        v->setVisited(false);
    }

    src->setDist(0);


    MutablePriorityQueue<Vertex> q;
    q.insert(src);

    while(!q.empty()){
        Vertex* v = q.extractMin();
        v->setVisited(true);
        for (Edge* e: v->getAdj()){
            Vertex* w = e->getDest();
            int residual = e->getResidualCapacity();
            if (!w->isVisited() && residual > 0){
                double oldDist = w->getDist();
                if (e->getCost() < oldDist){
                    w->setDist(e->getCost());
                    w->setPath(e);
                    if(oldDist == INF)
                        q.insert(w);
                    else
                        q.decreaseKey(w);
                }
            }
        }
    }
    return dest->isVisited();
}
int Graph::pathCost(Vertex* src, Vertex* dest){
    int cost = 0;
    for(Vertex* v = dest; v != src;){
        Edge* e = v->getPath();
        cost += e->getCost();
        v = e->getOrig();
    }
    return cost;
}
int Graph::minCost(int source, int target) {

    Vertex* src = findVertex(source);
    Vertex* dest = findVertex(target);
    if (src == nullptr || dest == nullptr || src == dest)
        return 0;

    int flow = 0, cost = 0;

    for (Vertex* vertex : vertexSet)
        for (Edge* edge : vertex->getAdj())
            edge->setResidualCapacity(edge->getCapacity());

    int i = 1;

    while (findMinCostAugmentingPath(src,dest)) {
        auto f = findMinResidualAlongPath(src, dest);
        augmentFlowAlongPath(src, dest, f);
        cout << "Path nr. " << i++ << " : ";
        printPath(src,dest);
        cout << endl;
        flow += f;
        cost += pathCost(src,dest);
    }
    return cost;

}