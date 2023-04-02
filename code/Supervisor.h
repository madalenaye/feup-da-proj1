
#ifndef DA_SUPERVISOR_H
#define DA_SUPERVISOR_H


#include <fstream>
#include <istream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "Graph.h"
#include "Station.h"
using namespace std;

class Supervisor{
public:
    Supervisor();

    unordered_map<string, int> getId() const;
    Graph getGraph() const;
    bool isStation(string station);
private:

    void createStations();
    void createGraph();
    Station::StationH stations;
    unordered_map<string, int> idStations;
    Graph graph;


    string removeQuotes(istringstream &iss, string field);

    void checkField(istringstream &iss, string &field);

    int makeVertex(string name, int &id);

    void removeUnusedStations();
};


#endif //DA_SUPERVISOR_H
