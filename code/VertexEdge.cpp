#include "VertexEdge.h"

/************************* Vertex  **************************/

Station Vertex::getStation() const {
    return station;
}

Vertex::Vertex(int id): id(id) {}

Edge * Vertex::addEdge(Vertex *d, int capacity, string service) {
    auto newEdge = new Edge(this, d, capacity, service);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

bool Vertex::removeEdge(int destID) {
    for (auto it = adj.begin(); it != adj.end(); it++) {
        Edge *edge = *it;
        Vertex *dest = edge->getDest();
        if (dest->getId() == destID) {
            adj.erase(it);
            for(auto it2 = dest->adj.begin(); it2 != dest->adj.end(); it2++) {
                dest->adj.erase(it2);
            }
            delete edge;
            return true;
        }
    }
    return false;
}

bool Vertex::operator<(Vertex & vertex) const {
    return this->dist < vertex.dist;
}

int Vertex::getId() const {
    return this->id;
}

vector<Edge*> Vertex::getAdj() const {
    return this->adj;
}

bool Vertex::isVisited() const {
    return this->visited;
}

bool Vertex::isProcessing() const {
    return this->processing;
}

unsigned int Vertex::getIndegree() const {
    return this->indegree;
}

int Vertex::getDist() const {
    return this->dist;
}

Edge *Vertex::getPath() const {
    return this->path;
}

vector<Edge *> Vertex::getIncoming() const {
    return this->incoming;
}

void Vertex::setId(int id) {
    this->id = id;
}

void Vertex::setVisited(bool visited) {
    this->visited = visited;
}

void Vertex::setProcesssing(bool processing) {
    this->processing = processing;
}

void Vertex::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

void Vertex::setDist(int dist) {
    this->dist = dist;
}

void Vertex::setPath(Edge *path) {
    this->path = path;
}

void Vertex::setStation(Station station) {
    this->station = station;
}

/********************** Edge  ****************************/

Edge::Edge(Vertex *orig, Vertex *dest, int capacity, string service): orig(orig), dest(dest), capacity(capacity), service(service) {
    if (service == "STANDARD")
        cost = 2;
    else
        cost = 4;
}

void Edge::setResidualCapacity(int residualCapacity) {
    this->residualCapacity = residualCapacity;
}

int Edge::getResidualCapacity() const {
    return this->residualCapacity;
}
void Edge::setCost(int cost){
    this->cost = cost;
}

int Edge::getCost() const{
    return this->cost;
}

Vertex * Edge::getDest() const {
    return this->dest;
}

int Edge::getCapacity() const {
    return this->capacity;
}

string Edge::getService() const{
    return this->service;
}

Vertex * Edge::getOrig() const {
    return this->orig;
}

Edge *Edge::getReverse() const {
    return this->reverse;
}

bool Edge::isSelected() const {
    return this->selected;
}

int Edge::getFlow() const {
    return flow;
}

void Edge::setSelected(bool selected) {
    this->selected = selected;
}

void Edge::setReverse(Edge *reverse) {
    this->reverse = reverse;
}

void Edge::setFlow(int flow) {
    this->flow = flow;
}