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

bool Supervisor::isStation(const string& station){
    return idStations.find(station) != idStations.end();
}

bool Supervisor::isLine(const string& line){
    return lines.find(line) != lines.end();
}

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

string Supervisor::removeQuotes(istringstream& iss, string field){
    string s = field.substr(1);

    while (!iss.eof() && s.back() != '\"') {
        getline(iss, field, ',');
        s += ',' + field;
    }
    return s.substr(0, s.length() - 1);
}

void Supervisor::checkField(istringstream& iss, string& field){
    getline(iss,field,',');
    if (field[0] == '\"')
        field = removeQuotes(iss, field);
}

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

void Supervisor::createSubgraph(const unordered_set<string>& failureLines) {

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

        if (!lineFailure(failureLines,source,target)) {
            idA = makeVertex(subGraph,subGraphStations, source, id);
            idB = makeVertex(subGraph,subGraphStations,target, id);
            subGraph.addEdge(idA, idB, capacity, service);
        }
    }
    inFile.close();
}

void Supervisor::createSubgraph(const Station::StationH& failureStations) {

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

        if (!stationFailure(failureStations,source,target)){
            idA = makeVertex(subGraph,subGraphStations, source, id);
            idB = makeVertex(subGraph,subGraphStations,target, id);
            subGraph.addEdge(idA, idB, capacity, service);
        }
    }
    inFile.close();
}

void Supervisor::createSubgraph(const vector<pair<string,string>>& failureSegments) {

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

        if (!segmentFailure(failureSegments,source,target)) {
            idA = makeVertex(subGraph,subGraphStations, source, id);
            idB = makeVertex(subGraph,subGraphStations,target, id);
            subGraph.addEdge(idA, idB, capacity, service);
        }
    }
    inFile.close();
}

bool Supervisor::stationFailure(Station::StationH failureStations, const string& source, const string& target){
    return (failureStations.find(source) != failureStations.end() || failureStations.find(target) != failureStations.end());
}

bool Supervisor::lineFailure(unordered_set<string> failureLines, const string& source, const string& target){
    Station src = *stations.find(source);
    Station dest = *stations.find(target);
    return (failureLines.find(src.getLine()) != failureLines.end() || failureLines.find(dest.getLine()) != failureLines.end());
}

bool Supervisor::segmentFailure(const vector<pair<string,string>>& failureSegments, const string& source, const string& target){
    pair<string, string> segment = {source,target};
    for (const auto& pair: failureSegments)
        if (pair.first == source && pair.second == target)
            return true;
    return false;
}