#include "supervisor.h"

Supervisor::Supervisor() {
    createStations();
    createGraph();
}

void Supervisor::createGraph() {
    ifstream inFile;
    string stationA, stationB, service, line, x;
    int capacity;
    inFile.open("../data/network.csv");
    getline(inFile, line);
    while(getline(inFile, line)){
        istringstream is(line);
        getline(is,stationA,',');
        getline(is,stationB,',');
        getline(is,x,','); capacity = stoi(x);
        getline(is,service,',');
        if (service.back() == '\r') service.pop_back();
        graph.addEdge(idStations[stationA],idStations[stationB], capacity, service);
    }
    inFile.close();
}

void Supervisor::createStations() {

    ifstream myFile;
    string currentLine, name, district, municipality, township, line;
    int i = 1;
    myFile.open("../data/stations.csv");

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
                line = field;
                if (line.back() == '\r') line.pop_back();
                Station station(name,district,municipality,township,line);
                fieldNr = -1;

                //stations.insert(station);
                idStations.insert({station.getName(),i});
                graph.addVertex(i++, station);
            }
            fieldNr++;
        }
    }
    myFile.close();
}

unordered_map<string, int> Supervisor::getId() {
    return idStations;
}