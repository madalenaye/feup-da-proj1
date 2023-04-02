#include "supervisor.h"

Supervisor::Supervisor() {
    createStations();
    createGraph();
}
int Supervisor::makeVertex(string name, int &id) {
    auto it = idStations.find(name);
    int stationId;
    if (it == idStations.end()) {
        stationId = id++;
        graph.addVertex(stationId, *stations.find(name));
        idStations[name] = stationId;
    } else {
        stationId = it->second;
    }
    return stationId;
}
void Supervisor::createGraph() {
    ifstream inFile;
    string source, target, service, line, x;
    int capacity, idA, idB, id=0;
    inFile.open("../data/network2.csv");
    getline(inFile, line);

    while(getline(inFile, line)) {

        istringstream is(line);

        checkField(is, source);
        checkField(is,target);
        checkField(is, x); capacity = stoi(x);
        checkField(is, service);

        if (service.back() == '\r') service.pop_back();

        idA = makeVertex(source, id);
        idB = makeVertex(target, id);

        graph.addEdge(idA, idB, capacity, service);
    }

    inFile.close();

    removeUnusedStations();
}

void Supervisor::removeUnusedStations(){
    for (auto it = stations.begin(); it != stations.end();){
        if (idStations.find(it->getName()) == nullptr)
            it = stations.erase(it);
        else it++;
    }
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

    }
    myFile.close();
}

unordered_map<string, int> Supervisor::getId() const{
    return idStations;
}

Graph Supervisor::getGraph() const {
    return graph;
}

bool Supervisor::isStation(string station){
    return idStations.find(station) != idStations.end();
}