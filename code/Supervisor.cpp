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

/**
 * This function checks if a station is present in the stations unordered_set.
 * @param station The name of the station.
 * @return True if the station exists, false otherwise.
 * @par Time complexity
 * O(1)
 */
bool Supervisor::isStation(const std::string& station){
    return stations.find(station) != stations.end();
}

/**
 * This function checks if a line is present in the lines unordered_set.
 * @param line The name of the line.
 * @return True if the line exists, false otherwise.
 * @par Time complexity
 * O(1)
 */
bool Supervisor::isLine(const std::string& line){
    return lines.find(line) != lines.end();
}

/**
 * This function creates a new vertex in the graph for the station with the given name,
 * or returns the existing vertex ID if the station already exists in the graph.
 *
 * @param _graph  The graph in which the vertex is to be created (main graph or subgraph).
 * @param ids  The unordered map that contains the station names and their corresponding vertex IDs.
 * @param name  the name of the station to create in the graph.
 * @param id  the ID of the next vertex to be created in the graph
 *
 * @return the vertex ID of the newly created or existing vertex
 *
 * @par Time complexity
 * O(log(n)), where n is the number of elements in the unordered map ids
 */
int Supervisor::makeVertex(Graph& _graph, std::unordered_map<std::string, int>& ids, const std::string& name, int &id) {
    auto it = ids.find(name);
    int stationId;
    if (it == ids.end()) {
        stationId = id++;
        _graph.addVertex(stationId, *stations.find(name));
        ids[name] = stationId;
    } else {
        stationId = it->second;
    }
    return stationId;
}

/**
 * This function removes quotes from a string field that contains a quoted string.
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
 * This function extracts the next field from an istringstream object and checks if it needs quotes to be removed.
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
 * This function reads the stations data from the CSV file "stations.csv" and creates Station objects and data structures for them,
 * namely an unordered set of stations (stations), an unordered set of their main lines (lines), and an unordered map that contains
 * the line names and their corresponding stations.
 *
 * @par Time complexity
 * O(n), where n is the number of lines in the CSV file
 */
void Supervisor::createStations() {

    std::ifstream myFile;
    std::string currentLine, name, district, municipality, township, line;

   myFile.open("../data/stations.csv");
   // myFile.open("../data/stations2.csv");
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
        stationsPerLine[line].insert(station);
        municipalityStations[municipality].insert(station);
        districtStations[district].insert(station);
    }
    myFile.close();
}

/**
 * This function reads and processes the CSV file "network.csv", which contains information about the
 * connections between the different stations. A graph of the railway network is created, where each station is
 * represented as a vertex and each connection is represented as an edge.
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
    //inFile.open("../data/network2.csv");
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
 * This function creates a subgraph of the main graph based on a set of failure lines, which represents transit lines
 * that are currently out of service due to repairs, services, disasters, or other reasons. The new graph excludes any
 * edges that belong to the failed lines.
 *
 * @param failedLines The set of lines to be excluded.
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
    //inFile.open("../data/network2.csv");
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

Graph Supervisor::subgraph(const std::vector<std::pair<std::string, std::string>>& failedSegments){

    Graph _subGraph;
    subGraphStations.clear();
    std::ifstream inFile;
    std::string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network.csv");
    //inFile.open("../data/network2.csv");
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



Graph Supervisor::subgraph(const Station::StationH& failedStations){

    Graph _subGraph;
    subGraphStations.clear();
    std::ifstream inFile;
    std::string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network.csv");
    //inFile.open("../data/network2.csv");
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
 * are in the set of failed stations.
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
 * This function checks if either the source or the target stations of a given edge
 * are in the set of failed lines.
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
 * This function checks if a given edge is in the set of failed segments.
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


void Supervisor::createSuperSource(int id, Station::StationH targetStations){

    std::vector<int> sources;
    for (auto v: graph.getVertexSet()){
        if (targetStations.find(v->getStation()) != targetStations.end()) continue;

        if (v->getAdj().size()==1){
        //if (v->getStation().getName() == "A" || v->getStation().getName() == "B" ) {
            sources.push_back(v->getId());
        }
    }

    superGraph.addVertex(id, Station("Super-Source"));
    superGraphStations["Super-Source"] = id;
    for(auto i:sources)
        superGraph.addEdge(id,i,INF,"");

}

void Supervisor::createSuperSink(int id, Station::StationH targetStations){

    std::vector<int> targets;
    for (auto v: graph.getVertexSet()){
        if (targetStations.find(v->getStation()) != targetStations.end())
            targets.push_back(v->getId());

    }
    superGraphStations["Super-Sink"] = id;
    superGraph.addVertex(id, Station("Super-Sink"));
    for(auto i:targets)
        superGraph.addEdge(i,id,INF,"");

}


void Supervisor::stationsFlow(){
    int flow;
    for (auto v: graph.getVertexSet()){
        createSuperSourceGraph(true,originalGraph(),v->getId());
        flow = superGraph.maxFlow(superGraphStations["Super-Source"], v->getId());
        stationFlow[v->getStation().getName()] = flow;
    }
};

int Supervisor::maxStationFlow(std::string station){
    return stationFlow[station];
}


int Supervisor::finalStationFlow(const Graph &_graph, int target){
    createSuperSourceGraph(false,_graph,target);
    return superGraph.maxFlow(superGraphStations["Super-Source"], target);
}

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

std::vector<std::pair<std::string, int>> Supervisor::maxConnectedStations(int type) {

    std::vector<std::pair<std::string, int>> res;
    if (type) {
        for (const auto& municipality: municipalityStations) {
            res.emplace_back(municipality.first, graph.maxConnectedMunicipality(municipality.first));
        }
    }
    else{
        for (const auto& district: districtStations) {
            res.emplace_back(district.first, graph.maxConnectedDistrict(district.first));
        }
    }
    sort(res.begin(), res.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b){
        return a.second > b.second; });
    return res;
}

void Supervisor::setSubGraph(const Graph& subgraph) {
    this->subGraph = subgraph;
}


void Supervisor::createSuperGraph(bool type, const Graph& _graph, const Station::StationH& targetStations){
    superGraph = _graph;
    superGraphStations = type ? idStations : subGraphStations;
    int id = superGraphStations.size();
    createSuperSource(id,targetStations);
    createSuperSink(id+1,targetStations);
}

void Supervisor::createSuperSourceGraph(bool type, const Graph& _graph, int target){
    superGraph = _graph;
    superGraphStations = type ? idStations : subGraphStations;
    std::vector<int> sources;
    for (auto v: superGraph.getVertexSet()){
        if (v->getId() == target) continue;

        if (v->getAdj().size() == 1){
        //if (v->getStation().getName() == "A" || v->getStation().getName() == "B" ) {
                sources.push_back(v->getId());
            }
    }
    int id = superGraphStations.size();
    superGraph.addVertex(id, Station("Super-Source"));
    superGraphStations["Super-Source"] = id;

    for(auto i:sources)
        superGraph.addEdge(id,i,INF,"");

}
