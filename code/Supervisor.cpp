#include "Supervisor.h"

Supervisor::Supervisor() {
    createStations();
    graph = originalGraph();
    stationsFlow();
}

Station::StationH Supervisor::getStations() const{
    return stations;
}

std::unordered_map<std::string, int> Supervisor::getId() const{
    return idStations;
}

std::unordered_map<std::string, int> Supervisor::getSubGraphStations() const{
    return subGraphStations;
}

Graph Supervisor::getGraph() const {
    return graph;
}

Graph Supervisor::getSubGraph() const{
    return subGraph;
}

void Supervisor::setSubGraph(const Graph& subgraph) {
    this->subGraph = subgraph;
}

/**
 * Checks if input station belongs to the unordered_set of stations.\n\n
 * @param station The name of the station.
 * @return True if the station exists, false otherwise.
 * @par Time complexity
 * O(1)
 */
bool Supervisor::isStation(const std::string& station){
    return stations.find(station) != stations.end();
}

/**
 * Checks if a line is present in the unordered_set lines.\n\n
 * @param line The name of the line.
 * @return True if the line exists, false otherwise.
 * @par Time complexity
 * O(1)
 */
bool Supervisor::isLine(const std::string& line){
    return lines.find(line) != lines.end();
}

/**
 * Creates a new vertex in the graph for the station with the given name,
 * or returns the existing vertex ID if the station already exists in the graph.\n\n
 *
 * @param _graph  The graph in which the vertex is to be created (main graph or subgraph).
 * @param ids  The unordered map that contains the station names and their corresponding vertex IDs.
 * @param name  the name of the station to create in the graph.
 * @param id  the ID of the next vertex to be created in the graph
 *
 * @return the vertex ID of the newly created or existing vertex
 *
 * @par Time complexity
 * O(1)
 */
int Supervisor::makeVertex(Graph& _graph, std::unordered_map<std::string, int>& ids, const std::string& name, int &id) {
    auto it = ids.find(name);
    int stationId;
    if (it == ids.end()) {
        stationId = id++;
        _graph.addVertex(stationId, *stations.find(name));
        ids[name] = stationId;
    }
    else
        stationId = it->second;
    return stationId;
}

/**
 * This function removes quotes from a string field that contains a quoted string.\n\n
 *
 * @param iss  The istringstream object that contains the field to process.
 * @param field The string field containing a quoted string to process.
 *
 * @return a string containing the processed text with the quotes removed.
 *
 * @par Time complexity
 * O(n), where n is the length of the string field
 */
std::string Supervisor::removeQuotes(std::istringstream& iss, std::string field){
    std::string s = field.substr(1);

    while (!iss.eof() && s.back() != '\"') {
        getline(iss, field, ',');
        s += ',' + field;
    }
    return s.substr(0, s.length() - 1);
}

/**
 * Extracts the next field from an istringstream object and checks if it needs quotes to be removed.\n\n
 *
 * @param iss The istringstream object containing the input text
 * @param field The string field containing a quoted string to process.
 *
 * @par Time complexity
 * O(n), where n is the length of the string field
 */
void Supervisor::checkField(std::istringstream& iss, std::string& field){
    getline(iss,field,',');
    if (field[0] == '\"')
        field = removeQuotes(iss, field);
}

/**
 * Reads the stations' data from the CSV file "stations.csv" and creates Station objects and data structures for them,
 * namely an unordered set of stations (stations),
 * an unordered set of their main lines (lines),
 * an unordered map that contains the line names and their corresponding stations (lineStations),
 * an unordered map that contains the district names and their corresponding stations (districtStations),
 * an unordered map that contains the municipality names and their corresponding stations (municipalityStations).\n\n
 *
 * @par Time complexity
 * O(n), where n is the number of lines in the CSV file
 */
void Supervisor::createStations() {

    std::ifstream myFile;
    std::string currentLine, name, district, municipality, township, line;

    myFile.open("../data/stations.csv");
    getline(myFile, currentLine);

    while (getline(myFile,currentLine)){
        std::istringstream iss(currentLine);

        checkField(iss,name);
        checkField(iss,district);
        checkField(iss,municipality);
        checkField(iss,township);
        checkField(iss,line);

        if (line.back() == '\r') line.pop_back();
        if (line == "Rede Espanhola") continue;

        Station station(name,district,municipality,township,line);
        stations.insert(station);
        lines.insert(line);
        municipalityStations[municipality].insert(station);
        districtStations[district].insert(station);
    }
    myFile.close();
}

/**
 * This function reads and processes the CSV file "network.csv", which contains information about the
 * connections between the different stations. A graph of the railway network is created, where each station is
 * represented as a vertex and each connection is represented as an edge.\n\n
 *
 * @return graph created using the CSV file
 *
 * @par Time complexity
 * O(n), where n is the number of lines in the CSV file
 */
Graph Supervisor::originalGraph() {
    Graph _graph;
    idStations.clear();
    std::ifstream inFile;
    std::string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network.csv");
    getline(inFile, line);

    while(getline(inFile, line)) {

        std::istringstream is(line);

        checkField(is, source);
        checkField(is,target);
        checkField(is, x); capacity = stoi(x);
        checkField(is, service);

        if (service.back() == '\r') service.pop_back();

        idA = makeVertex(_graph,idStations,source, id);
        idB = makeVertex(_graph,idStations,target, id);

        _graph.addEdge(idA, idB, capacity, service);
    }

    inFile.close();
    return _graph;
}

/**
 * This function creates a subgraph of the main graph based on a set of failed lines, which represents transit lines
 * that are currently out of service due to repairs, services, disasters, or other reasons. The new graph excludes any
 * edges that belong to the failed lines.\n\n
 *
 * @param failedLines The set of lines to be excluded.
 *
 * @return subgraph of the original graph
 *
 * @par Time complexity
 * O(n), where n is the number of lines in the CSV file
 */
Graph Supervisor::subgraph(const std::unordered_set<std::string>& failedLines){

    Graph _subGraph;
    subGraphStations.clear();
    std::ifstream inFile;
    std::string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network.csv");
    getline(inFile, line);

    while(getline(inFile, line)) {

        std::istringstream is(line);

        checkField(is, source);
        checkField(is,target);

        checkField(is, x); capacity = stoi(x);
        checkField(is, service);

        if (service.back() == '\r') service.pop_back();

        if (!lineFailure(failedLines,source,target)) {
            idA = makeVertex(_subGraph,subGraphStations, source, id);
            idB = makeVertex(_subGraph,subGraphStations,target, id);
            _subGraph.addEdge(idA, idB, capacity, service);
        }
    }
    inFile.close();
    return _subGraph;
}

/**
 * This function creates a subgraph of the main graph based on a set of failed segments, which represents transit segments
 * that are currently out of service due to repairs, services, disasters, or other reasons. The new graph excludes any
 * edges that belong to the failed segments.\n\n
 *
 * @param failedSegments The set of segments to be excluded.
 *
 * @return subgraph of the original graph
 *
 * @par Time complexity
 * O(n), where n is the number of segments in the CSV file
 */
Graph Supervisor::subgraph(const std::vector<std::pair<std::string, std::string>>& failedSegments){

    Graph _subGraph;
    subGraphStations.clear();
    std::ifstream inFile;
    std::string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network.csv");
    getline(inFile, line);

    while(getline(inFile, line)) {

        std::istringstream is(line);

        checkField(is, source);
        checkField(is,target);

        checkField(is, x); capacity = stoi(x);
        checkField(is, service);

        if (service.back() == '\r') service.pop_back();

        if (!segmentFailure(failedSegments,source,target)) {
            idA = makeVertex(_subGraph,subGraphStations, source, id);
            idB = makeVertex(_subGraph,subGraphStations,target, id);
            _subGraph.addEdge(idA, idB, capacity, service);
        }
    }
    inFile.close();
    return _subGraph;
}

/**
 * This function creates a subgraph of the main graph based on a set of failed stations, which represents stations
 * that are currently out of service due to repairs, services, disasters, or other reasons. The new graph excludes any
 * edges that belong to the failed stations.\n\n
 *
 * @param failedStations The set of stations to be excluded.
 *
 * @return subgraph of the original graph
 *
 * @par Time complexity
 * O(n), where n is the number of stations in the CSV file
 */
Graph Supervisor::subgraph(const Station::StationH& failedStations){

    Graph _subGraph;
    subGraphStations.clear();
    std::ifstream inFile;
    std::string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network.csv");
    getline(inFile, line);

    while(getline(inFile, line)) {

        std::istringstream is(line);

        checkField(is, source);
        checkField(is,target);

        checkField(is, x); capacity = stoi(x);
        checkField(is, service);

        if (service.back() == '\r') service.pop_back();

        if (!stationFailure(failedStations,source,target)) {
            idA = makeVertex(_subGraph,subGraphStations, source, id);
            idB = makeVertex(_subGraph,subGraphStations,target, id);
            _subGraph.addEdge(idA, idB, capacity, service);
        }
    }
    inFile.close();
    return _subGraph;
}

/**
 * This function checks if either the source or the target stations of a given edge
 * are in the set of failed stations.\n\n
 *
 * @param failedStations The set of stations to be excluded.
 * @param source The name of the source station.
 * @param target The name of the target station.
 *
 * @return True if source or target is in the set failedStations, false otherwise.
 *
 * @par Time complexity
 * O(1)
 */
bool Supervisor::stationFailure(Station::StationH failedStations, const std::string& source, const std::string& target){
    return (failedStations.find(source) != failedStations.end() || failedStations.find(target) != failedStations.end());
}

/**
 * This function checks if both the source and the target stations of a given edge
 * are in the set of failed lines.\n\n
 *
 * @param failedLines The set of lines to be excluded.
 * @param source The name of the source station.
 * @param target The name of the target station.
 *
 * @return True if source or target is in the set failedLines, false otherwise.
 *
 * @par Time complexity
 * O(1)
 */
bool Supervisor::lineFailure(std::unordered_set<std::string> failedLines, const std::string& source, const std::string& target){
    Station src = *stations.find(source);
    Station dest = *stations.find(target);
    return (failedLines.find(src.getLine()) != failedLines.end() && failedLines.find(dest.getLine()) != failedLines.end());
}

/**
 * This function checks if a given edge is in the set of failed segments.\n\n
 *
 * @param failedSegments The vector of segments to be excluded.
 * @param source The name of the source station.
 * @param target The name of the target station.
 *
 * @return True if the edge is in failedSegments, false otherwise.
 *
 * @par Time complexity
 * O(n), where n is the number of elements of the vector failedSegments
 */
bool Supervisor::segmentFailure(const std::vector<std::pair<std::string,std::string>>& failedSegments, const std::string& source, const std::string& target){
    return std::any_of(failedSegments.begin(), failedSegments.end(),
       [&](const std::pair<std::string,std::string>& pair) {return pair.first == source && pair.second == target;});
}

/**
 * Calculates the flow of all the stations.\n\n
 *
 * @par Time complexity
 * O(V² * E²), where V is the number of vertexes and E the number of edges in the graph
 */
void Supervisor::stationsFlow(){
    int flow;
    for (auto v: graph.getVertexSet()){
        createSuperSourceGraph(true,originalGraph(),v->getId());
        flow = superGraph.maxFlow(superGraphStations["Super-Source"], v->getId());
        stationFlow[v->getStation().getName()] = flow;
    }
}

/**
 * This function returns the flow of a specific station.\n\n
 *
 * @param station wanted station
 *
 * @return max flow of a station
 *
 * @par Time complexity
 * O(1), where V is the number of vertexes and E the number of edges in the graph
 */
int Supervisor::maxStationFlow(const std::string& station){
    return stationFlow[station];
}

/**
 * Calculates the max flow of a specific station from the superGraph which contains a Super-Source connected
 * to all vertexes that have 1 outgoing edge.\n\n
 *
 * @param _graph wanted graph
 * @param target  wanted station id
 * @return  max flow of target station value
 *
 * @par Time complexity
 * O(V * E²), where V is the number of vertexes and E the number of edges in the graph
 */
int Supervisor::finalStationFlow(const Graph &_graph, int target){
    createSuperSourceGraph(false,_graph,target);
    return superGraph.maxFlow(superGraphStations["Super-Source"], target);
}

/**
 * This function calculates the reduction of flow created by failures of lines, stations or segments.\n\n
 *
 * @param _subGraph subGraph created because of line,station or segment failures
 * @return  vector containing all stations and the difference of flow affected by the failures
 *
 * @par Time complexity
 * O(V² * E²), where V is the number of vertexes and E the number of edges in the graph
 */
std::vector<std::pair<std::string,int>> Supervisor::flowDifference(const Graph& _subGraph){
    std::vector<std::pair<std::string,int>> res;
    int initial, final, difference;
    std::string station;

    for (auto vertex: subGraph.getVertexSet()){
        station = vertex->getStation().getName();
        initial = stationFlow[station];
        final = finalStationFlow(_subGraph,vertex->getId());
        difference = initial - final;
        res.emplace_back(vertex->getStation().getName(), difference);
    }

    sort(res.begin(), res.end(), [](const std::pair<std::string,int>& a, const std::pair<std::string, int>& b){
        return a.second > b.second;
    });

    return res;
}

/**
 * This function calculates the max flow of each district/municipality and then orders it by descending flow, using the
 * superGraph that has a Super-Source (connected to all the vertexes with only 1 edge) and a Super-Sink connected to
 * each target station.\n\n
 *
 * @param graphType
 * @param _graph main graph
 * @param type 1 if the user wants municipality or 2 if district
 *
 * @return vector of pairs municipality/distric its max flow, ordered in descending order
 *
 * @par Time complexity
 * O(n * (V * E²)), where V is the number of vertexes and E the number of edges in the graph n the size of municipality/district unordered maps
 */
std::vector<std::pair<std::string,int>> Supervisor::transportNeeds(bool graphType, const Graph& _graph, bool type){
    auto _stations = type ? municipalityStations : districtStations;

    std::vector<std::pair<std::string, int>> res;

    for (const auto& pair : _stations){
        createSuperGraph(graphType,_graph,pair.second);

        int maxFlow = superGraph.maxFlow(superGraphStations["Super-Source"],superGraphStations["Super-Sink"]);
        res.emplace_back(pair.first,maxFlow);
    }

    sort(res.begin(), res.end(), [](const std::pair<std::string,int>& a, const std::pair<std::string, int>& b){
        return a.second > b.second;
    });

    return res;
}

/**
 * This function calculates the maximum number of connected stations in a municipality or district and orders them by descending order in
 * a vector.\n\n
 * @return vector of pairs,municipality/district and maximum connected components ordered in descending order
 *
 * @par Time complexity
 * O(n * (V+E)), where V is the number of vertexes and E the number of edges and n the size of municipality/district unordered maps
 */
std::vector<std::pair<std::string, int>> Supervisor::maxConnectedStations(int type) {

    std::vector<std::pair<std::string, int>> res;
    if (type)
        for (const auto& municipality: municipalityStations)
            res.emplace_back(municipality.first, graph.maxConnectedMunicipality(municipality.first));
    else
        for (const auto& district: districtStations)
            res.emplace_back(district.first, graph.maxConnectedDistrict(district.first));

    sort(res.begin(), res.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b){
        return a.second > b.second; });

    return res;
}

/**
 * This function adds a super source to the superGraph. It connects itself with all the vertexes/stations that have only 1 outgoing
 * edge.\n\n
 *
 * @param id super source node id
 * @param targetStations unordered set with all the target stations
 *
 * @par Time complexity
 * O(V), where V is the number of vertexes in the graph
 */
void Supervisor::createSuperSource(int id, Station::StationH targetStations){
    superGraph.addVertex(id, Station("Super-Source"));
    superGraphStations["Super-Source"] = id;
    for (auto v: graph.getVertexSet()){
        if (targetStations.find(v->getStation()) != targetStations.end())
            continue;
        if (v->getAdj().size()==1)
            superGraph.addEdge(id,v->getId(),INF,"");
    }
}

/**
 * This function creates a super sink node and then connects it to all the target stations.\n\n
 *
 * @param id super sink id
 * @param targetStations unordered set containing all the stations that are needed
 *
 * @par Time complexity
 * O(V), where V is the number of vertexes in the graph
 */
void Supervisor::createSuperSink(int id, Station::StationH targetStations){

    std::vector<int> targets;
    for (auto v: graph.getVertexSet())
        if (targetStations.find(v->getStation()) != targetStations.end())
            targets.push_back(v->getId());

    superGraphStations["Super-Sink"] = id;
    superGraph.addVertex(id, Station("Super-Sink"));

    for(auto i:targets)
        superGraph.addEdge(i,id,INF,"");

}

/**
 * This function creates a graph  that  has a Super-Source connected to
 * all the stations that have only 1 outgoing edge and a Super-Sink connected to all stations in the targetStations.\n\n
 *
 * @param type
 * @param _graph
 * @param targetStations unordered map containing all target stations
 *
 * @par Time complexity
 * O(V), where V is the number of vertexes in the graph
 */
void Supervisor::createSuperGraph(bool type, const Graph& _graph, const Station::StationH& targetStations){
    superGraph = _graph;
    superGraphStations = type ? idStations : subGraphStations;
    int id = superGraphStations.size();
    createSuperSource(id,targetStations);
    createSuperSink(id+1,targetStations);
}

/**
 * This function creates a graph that only has a Super-Source connected to all
 * vertexes with only 1 outgoing edge.\n\n
 *
 * @param type
 * @param _graph
 * @param target
 *
 * @par Time complexity
 * O(V), where V is the number of vertexes in the graph
 */
void Supervisor::createSuperSourceGraph(bool type, const Graph& _graph, int target){
    superGraph = _graph;
    superGraphStations = type ? idStations : subGraphStations;
    int id = superGraphStations.size();
    superGraph.addVertex(id, Station("Super-Source"));
    superGraphStations["Super-Source"] = id;
    for (auto v: superGraph.getVertexSet()){
        if (v->getId() == target)
            continue;
        if (v->getAdj().size()==1)
            superGraph.addEdge(id,v->getId(),INF,"");
    }
}