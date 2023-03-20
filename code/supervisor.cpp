#include "supervisor.h"

Supervisor::Supervisor() {
    createStations();
    createGraph();
}

void Supervisor::createGraph() {
    ifstream inFile;
    string source, target, service, line, x;
    Station stationA(""), stationB("");
    int capacity, idA, idB, i=1;
    inFile.open("../data/network.csv");
    getline(inFile, line);
    while(getline(inFile, line)){
        istringstream is(line);
        getline(is,source,',');
        getline(is,target,',');
        getline(is,x,','); capacity = stoi(x);
        getline(is,service,',');

        if (service.back() == '\r') service.pop_back();

        stationA = *stations.find(source);
        stationB = *stations.find(target);

        if ((idStations.find(source)) == nullptr){
            idA = i++;
            graph.addVertex(idA,stationA);
            idStations.insert({source,idA});
        }
        else {
            idA = idStations[source];
        }
        if ((idStations.find(target)) == nullptr){
            idB = i++;
            graph.addVertex(idB,stationB);
            idStations.insert({target,idB });
        }
        else {
            idB = idStations[target];
        }

        graph.addEdge(idA,idB, capacity, service);
    }
    inFile.close();
    for (auto it = stations.begin(); it != stations.end();){
        if (idStations.find(it->getName()) == nullptr)
            it = stations.erase(it);
        else it++;
    }


}

void Supervisor::createStations() {

    ifstream myFile;
    string currentLine, name, district, municipality, township, line;
    int i = 1;
    myFile.open("../data/stations.csv");
    graph.addVertex(0,Station(""));
    getline(myFile, currentLine);
    int fieldNr = 0;
    while (getline(myFile,currentLine)){
        istringstream iss(currentLine);
        string field;

        while (getline(iss, field, ',')) {
            if (fieldNr == 0) {
                name = field;
            }
            else if (fieldNr == 1) {
                district = field;
            }
            else if (fieldNr == 2) {
                municipality = field;
            }
            else if (fieldNr == 3) {
                if (field[0] == '\"') {
                    township = field.substr(1);
                    while (!iss.eof()) {
                        getline(iss, field, ',');
                        township += ',' + field;
                        if (field[field.length() - 1] == '\"') {
                            break;
                        }
                    }
                    township = township.substr(0, township.length() - 1);
                } else {
                    township = field;
                }
            }
            else if (fieldNr == 4) {
                fieldNr = -1;
                line = field;
                if (line.back() == '\r') line.pop_back();
                if (line == "Rede Espanhola") {
                    fieldNr = 0;
                    continue;
                }
                Station station(name,district,municipality,township,line);
                stations.insert(station).second;
            }
            fieldNr++;
        }
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