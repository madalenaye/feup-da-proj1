#include "Supervisor.h"

Supervisor::Supervisor() {
    createStations();
    createGraph();
}


Station::StationH Supervisor::getStations() const{
    return stations;
}

unordered_map<string, int> Supervisor::getId() const{
    return idStations;
}

unordered_map<string, int> Supervisor::getSubGraphStations() const{
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
bool Supervisor::isStation(const string& station){
    return stations.find(station) != stations.end();
}

/**
 * This function checks if a line is present in the lines unordered_set.
 * @param line The name of the line.
 * @return True if the line exists, false otherwise.
 * @par Time complexity
 * O(1)
 */
bool Supervisor::isLine(const string& line){
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
int Supervisor::makeVertex(Graph& _graph, unordered_map<string, int>& ids, const string& name, int &id) {
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
string Supervisor::removeQuotes(istringstream& iss, string field){
    string s = field.substr(1);

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
void Supervisor::checkField(istringstream& iss, string& field){
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

    ifstream myFile;
    string currentLine, name, district, municipality, township, line;

    myFile.open("../data/stations.csv");

    getline(myFile, currentLine);

    while (getline(myFile,currentLine)){
        istringstream iss(currentLine);

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
void Supervisor::createGraph() {
    ifstream inFile;
    string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network.csv");
    getline(inFile, line);

    while(getline(inFile, line)) {

        istringstream is(line);

        checkField(is, source);
        checkField(is,target);
        checkField(is, x); capacity = stoi(x);
        checkField(is, service);

        if (service.back() == '\r') service.pop_back();

        idA = makeVertex(graph,idStations,source, id);
        idB = makeVertex(graph,idStations,target, id);

        graph.addEdge(idA, idB, capacity, service);
    }

    inFile.close();
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
void Supervisor::createSubgraph(const unordered_set<string>& failedLines) {

    this->subGraph = Graph();
    subGraphStations.clear();

    ifstream inFile;
    string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network.csv");
    getline(inFile, line);

    while(getline(inFile, line)) {

        istringstream is(line);

        checkField(is, source);
        checkField(is,target);

        checkField(is, x); capacity = stoi(x);
        checkField(is, service);

        if (service.back() == '\r') service.pop_back();

        if (!lineFailure(failedLines,source,target)) {
            idA = makeVertex(subGraph,subGraphStations, source, id);
            idB = makeVertex(subGraph,subGraphStations,target, id);
            subGraph.addEdge(idA, idB, capacity, service);
        }
    }
    inFile.close();
}

/**
 * This function creates a subgraph of the main graph based on a set of failure stations, which represents stations
 * that are currently out of service due to repairs, services, disasters, or other reasons. The new graph excludes any
 * stations that belong to the failed stations.
 *
 * @param failedStations The set of stations to be excluded.
 *
 * @par Time complexity
 * O(n), where n is the number of lines in the CSV file
 */
void Supervisor::createSubgraph(const Station::StationH& failedStations) {

    this->subGraph = Graph();
    subGraphStations.clear();

    ifstream inFile;
    string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network.csv");
    getline(inFile, line);

    while(getline(inFile, line)) {

        istringstream is(line);

        checkField(is, source);
        checkField(is,target);

        checkField(is, x); capacity = stoi(x);
        checkField(is, service);

        if (service.back() == '\r') service.pop_back();

        if (!stationFailure(failedStations,source,target)){
            idA = makeVertex(subGraph,subGraphStations, source, id);
            idB = makeVertex(subGraph,subGraphStations,target, id);
            subGraph.addEdge(idA, idB, capacity, service);
        }
    }
    inFile.close();
}

/**
 * This function creates a subgraph of the main graph based on a set of failure segments, which represents segments
 * that are currently out of service due to repairs, services, disasters, or other reasons. The new graph excludes any
 * edges that belong to the failed segments.
 *
 * @param failedStations The set of segments to be excluded.
 *
 * @par Time complexity
 * O(n), where n is the number of lines in the CSV file
 */
void Supervisor::createSubgraph(const vector<pair<string,string>>& failedSegments) {

    this->subGraph = Graph();
    subGraphStations.clear();

    ifstream inFile;
    string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network.csv");
    getline(inFile, line);

    while(getline(inFile, line)) {

        istringstream is(line);

        checkField(is, source);
        checkField(is,target);

        checkField(is, x); capacity = stoi(x);
        checkField(is, service);

        if (service.back() == '\r') service.pop_back();

        if (!segmentFailure(failedSegments,source,target)) {
            idA = makeVertex(subGraph,subGraphStations, source, id);
            idB = makeVertex(subGraph,subGraphStations,target, id);
            subGraph.addEdge(idA, idB, capacity, service);
        }
    }
    inFile.close();
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
bool Supervisor::stationFailure(Station::StationH failedStations, const string& source, const string& target){
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
bool Supervisor::lineFailure(unordered_set<string> failedLines, const string& source, const string& target){
    Station src = *stations.find(source);
    Station dest = *stations.find(target);
    return (failedLines.find(src.getLine()) != failedLines.end() || failedLines.find(dest.getLine()) != failedLines.end());
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
bool Supervisor::segmentFailure(const vector<pair<string,string>>& failedSegments, const string& source, const string& target){
    return std::any_of(failedSegments.begin(), failedSegments.end(),
       [&](const pair<string,string>& pair) {return pair.first == source && pair.second == target;});
}