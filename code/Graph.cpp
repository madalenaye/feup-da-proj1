#include "Graph.h"

Vertex * Graph::findVertex(const int &id) const {
    return vertexSet[id];
}

void Graph::addVertex(const int &id, Station station) {
    auto* v = new Vertex(id);
    v->setStation(std::move(station));
    vertexSet.push_back(v);
}

void Graph::addEdge(const int &source, const int &dest, int capacity, const std::string& service) const {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return;
    auto e1 = v1->addEdge(v2, capacity/2, service);
    auto e2 = v2->addEdge(v1, capacity/2, service);
    e1->setReverse(e2);
    e2->setReverse(e1);
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return this->vertexSet;
}

/**
 * @brief Finds the shortest augmenting path from the source to the target using a BFS.
 *
 * This function finds the shortest augmenting path from the source vertex to the target vertex
 * in the graph using a breadth-first search algorithm.\n\n
 *
 * @param src A pointer to the source vertex.
 * @param dest A pointer to the target vertex.
 *
 * @return True if a path from the source to the target was found, false otherwise.
 *
 * @par Time complexity
 * O(V + E), where V is the number of vertexes and E the number of edges in the graph.
 */
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
            if (w->getStation().getName() == "Super-Source") continue;
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

/**
 * @brief Finds the minimum residual capacity along the chosen augmenting path from the source to the target.
 *
 * This function finds the minimum residual capacity along the chosen augmenting path from the source
 * vertex to the target vertex in the graph, by examining each edge along the path and
 * returning the smallest residual capacity.\n\n
 *
 * @param src A pointer to the source vertex.
 * @param dest A pointer to the target vertex.
 *
 * @return The bottleneck capacity of the chosen augmenting path from the source to the target.
 *
 * @par Time complexity
 * O(V + E), where V is the number of vertexes and E the number of edges in the graph.
 */
int Graph::findMinResidualAlongPath(Vertex* src, Vertex* dest){
    int f = INF;
    for(Vertex* v = dest; v != src;){
        Edge* e = v->getPath();
        f = std::min(f,e->getResidualCapacity());
        v = e->getOrig();
    }
    return f;
}

/**
 * @brief Augments the flow along the chosen augmenting path from the source to the target.
 *
 * This function updates the flow along the chosen augmenting path from the source vertex to the
 * target vertex in the graph by adding or subtracting the flow amount `flow` from the residual
 * capacity of each edge along the path.\n\n
 *
 * @param src A pointer to the source vertex.
 * @param dest A pointer to the target vertex.
 * @param flow The amount of flow to augment along the path.
 *
 * @par Time complexity
 * O(V + E), where V is the number of vertexes and E the number of edges in the graph.
 */
void Graph::augmentFlowAlongPath(Vertex *src, Vertex *dest, int flow){
    for (Vertex* v = dest; v != src;){
        Edge* e = v->getPath();
        Edge* reverse = e->getReverse();

        e->setResidualCapacity(e->getResidualCapacity()-flow);
        reverse->setResidualCapacity(reverse->getResidualCapacity()+flow);

        v = e->getOrig();
    }
}

/**
 * @brief Finds the maximum flow from the source vertex to the target vertex using the Edmonds-Karp algorithm.
 *
 * This function implements the Edmonds-Karp algorithm to find the maximum flow from the source vertex to the target
 * vertex in the graph. The algorithm works by repeatedly finding the shortest augmenting path from the source to
 * the target using BFS, and then augmenting the flow along that path until no more augmenting paths exist. \n\n
 *
 * @param source The identifier of the source vertex.
 * @param target The identifier of the target vertex.
 *
 * @return The maximum flow from the source vertex to the target vertex.
 *
 * @par Time complexity
 * O(V * E²), where V is the number of vertexes and E the number of edges in the graph.
 */
int Graph::maxFlow(int source, int target){

    Vertex* src = findVertex(source);
    Vertex* dest = findVertex(target);
    if (src == nullptr || dest == nullptr || src == dest)
        return 0;

    int flow = 0;

    for (Vertex* vertex : vertexSet)
        for (Edge* edge : vertex->getAdj())
            edge->setResidualCapacity(edge->getCapacity());

    while (findAugmentingPath(src,dest)) {
        auto f = findMinResidualAlongPath(src, dest);
        augmentFlowAlongPath(src, dest, f);
        flow+=f;
    }

    return flow;
}

/**
 * @brief Finds the minimum cost augmenting path from the source vertex to the destination vertex.
 *
 * This function uses Dijkstra's algorithm with a priority queue to find the minimum cost augmenting
 * path from the source vertex to the destination vertex in the graph. The function updates the path
 * information for each visited vertex and returns a boolean indicating whether a path was found or not.\n\n
 *
 * @param src A pointer to the source vertex.
 * @param dest A pointer to the destination vertex.
 *
 * @return True if a minimum cost augmenting path was found, false otherwise.
 *
 * @par Time Complexity
 * O((V + E) * log(V)), where V is the number of vertexes and E the number of edges in the graph.
 */
bool Graph::findMinCostAugmentingPath(Vertex* src, Vertex* dest){
    for(Vertex* v: vertexSet){
        v->setPathCost(INF);
        v->setPath(nullptr);
        v->setVisited(false);
    }

    src->setPathCost(0);

    PriorityQueue q;
    q.insert(src);

    while(!q.empty()){
        Vertex* v = q.extractMin();
        v->setVisited(true);
        for (Edge* e: v->getAdj()){
            Vertex* w = e->getDest();
            int residual = e->getResidualCapacity();
            if (!w->isVisited() && residual > 0){
                int oldPathCost = w->getPathCost();
                int newPathCost = v->getPathCost() + e->getCost();
                if (newPathCost < oldPathCost){
                    w->setPathCost(newPathCost);
                    w->setPath(e);
                    if(oldPathCost == INF)
                        q.insert(w);
                    else
                        q.decreaseKey(w);
                }
            }
        }
    }
    return dest->isVisited();
}

/**
 * This function calculates the minimum cost maximum flow from the source vertex to the destination vertex using the
 * successive shortest path algorithm.\n\n
 *
 * @param source The identifier of the source vertex.
 * @param target The identifier of the target vertex.
 *
 * @return The minimum cost of the flow from the source to the target, or 0 if there is no feasible flow.
 *
 * @par Time complexity
 * O(V * E² * log(V)), where V is the number of vertexes and E the number of edges in the graph.
 */
int Graph::minCost(int source, int target) {

    Vertex* src = findVertex(source);
    Vertex* dest = findVertex(target);
    if (src == nullptr || dest == nullptr || src == dest)
        return 0;

    int cost = 0;

    for (Vertex* vertex : vertexSet)
        for (Edge* edge : vertex->getAdj())
            edge->setResidualCapacity(edge->getCapacity());

    while (findMinCostAugmentingPath(src,dest)) {
        auto f = findMinResidualAlongPath(src, dest);
        augmentFlowAlongPath(src, dest, f);
        cost += dest->getPathCost();
    }

    return cost;
}

/**
 * Stores the path of the stations of a specific district.\n\n
 *
 * @param v wanted vertex to see size of connected component
 * @param comp contains all connected vertexes
 * @param district wanted district
 *
 * @par Time complexity
 * O(V + E), where V is the number of vertexes and E the number of edges in the graph.
 */
void Graph::dfsConnectedDistrict(Vertex *v, std::list<int> &comp, const std::string& district) {
    v->setVisited(true);
    comp.push_back(v->getId());
    for (auto e : v->getAdj()){
        auto w = e->getDest();
        if (!w->isVisited() && w->getStation().getDistrict() == district)
            dfsConnectedDistrict(w, comp, district);
    }
}

/**
 * Stores the path of the stations of a specific municipality.\n\n
 *
 * @param v wanted vertex to see size of connected component
 * @param comp contains all connected vertexes
 * @param municipality wanted municipality
 *
 * @par Time complexity
 * O(V + E), where V is the number of vertexes and E the number of edges in the graph.
 */
void Graph::dfsConnectedMunicipality(Vertex *v, std::list<int> &comp, const std::string& municipality) {
    v->setVisited(true);
    comp.push_back(v->getId());
    for (auto e : v->getAdj()){
        auto w = e->getDest();
        if (!w->isVisited() && w->getStation().getMunicipality() == municipality)
            dfsConnectedMunicipality(w, comp, municipality);
    }
}

/**
 * This function calculates the largest connected component of a district using a depth-first search algorithm.\n\n
 *
 * @param district wanted district
 *
 * @return value of the largest connected component
 *
 * @par Time complexity
 * O(V + E), where V is the number of vertexes and E the number of edges in the graph.
 */
unsigned int Graph::maxConnectedDistrict(const std::string& district) {
    unsigned int maxSize = 0;

    for (auto v : vertexSet)
        v->setVisited(false);

    for (auto v  : vertexSet)
        if (!v->isVisited() && v->getStation().getDistrict() == district){
            std::list<int> components;
            dfsConnectedDistrict(v, components, district);
            unsigned int size = components.size();
            if (size > maxSize) maxSize = size;
        }

    return maxSize;
}

/**
 * This function calculates the largest connected component of a municipality using a depth-first search algorithm.\n\n
 *
 * @param municipality wanted municipality
 *
 * @return value of the largest connected component
 *
 * @par Time complexity
 * O(V + E), where V is the number of vertexes and E the number of edges in the graph.
 */
unsigned int Graph::maxConnectedMunicipality(const std::string& municipality) {
    unsigned int maxSize = 0;

    for (auto v : vertexSet)
        v->setVisited(false);

    for (auto v : vertexSet)
        if (!v->isVisited() && v->getStation().getMunicipality() == municipality){
            std::list<int> components;
            dfsConnectedMunicipality(v, components, municipality);
            unsigned int size = components.size();
            if (size > maxSize) maxSize = size;
        }

    return maxSize;
}