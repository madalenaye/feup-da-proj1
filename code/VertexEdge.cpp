#include "VertexEdge.h"

/************************* Vertex  **************************/


Edge * Vertex::addEdge(Vertex *d, int capacity, const std::string& service) {
    auto newEdge = new Edge(this, d, capacity, service);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

bool Vertex::operator<(Vertex & vertex) const {
    return this->dist < vertex.dist;
}

Station Vertex::getStation() const {
    return this->station;
}

int Vertex::getId() const {
    return this->id;
}

std::vector<Edge*> Vertex::getAdj() const {
    return this->adj;
}

bool Vertex::isVisited() const {
    return this->visited;
}

int Vertex::getDist() const {
    return this->dist;
}

Edge *Vertex::getPath() const {
    return this->path;
}

void Vertex::setVisited(bool visited) {
    this->visited = visited;
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

Edge::Edge(Vertex *orig, Vertex *dest, int capacity, const std::string& service): orig(orig), dest(dest), capacity(capacity), service(service) {
    if (service == "STANDARD")
        cost = 2;
    else if (service == "ALFA")
        cost = 4;
}

void Edge::setResidualCapacity(int residualCapacity) {
    this->residualCapacity = residualCapacity;
}

int Edge::getResidualCapacity() const {
    return this->residualCapacity;
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

Vertex * Edge::getOrig() const {
    return this->orig;
}

Edge *Edge::getReverse() const {
    return this->reverse;
}

void Edge::setReverse(Edge *reverse) {
    this->reverse = reverse;
}