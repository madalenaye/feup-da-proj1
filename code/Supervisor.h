
#ifndef DA_SUPERVISOR_H
#define DA_SUPERVISOR_H


#include <fstream>
#include <istream>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>


#include "Graph.h"
#include "Station.h"
using namespace std;

class Supervisor{
public:

    Supervisor();

    //getters
    Station::StationH getStations() const;
    unordered_map<string, int> getId() const;
    Graph getGraph() const;
    Graph getSubGraph() const;
    unordered_map<string, int> getSubGraphStations() const;

    //validate
    bool isStation(const string& station);
    bool isLine(const string& line);

    void createSubgraph(const unordered_set<string>& failedLines);
    void createSubgraph(const vector<pair<string, string>>& failedSegments);
    void createSubgraph(const Station::StationH& failedStations);


    vector<pair<string, int>> transportNeeds(bool type);
    vector<pair<string, int>> maxConnectedStations(int type);

    int maxStationFlow(int target);

    vector<pair<string, int>> flowDifference();

private:

    void createStations();
    void createGraph();
    void createSuperSource(int id, Station::StationH targetStations);
    void createSuperSink(int id, Station::StationH targetStations);
    void createSuperSourceGraph(int target);
    void createSuperGraph(const Station::StationH& targetStations);


    static void checkField(istringstream &iss, string &field);
    static string removeQuotes(istringstream &iss, string field);

    int makeVertex(Graph &_graph, unordered_map<string, int> &ids, const string &name, int &id);

    static bool segmentFailure(const vector<pair<string, string>>& failedSegments, const string& source, const string& target);
    bool lineFailure(unordered_set<string> failedLines, const string &source, const string &target);
    static bool stationFailure(Station::StationH failedStations, const string &source, const string &target);

    Station::StationH stations;
    unordered_map<string, int> idStations;

    unordered_map<string, Station::StationH> municipalityStations;
    unordered_map<string, Station::StationH> districtStations;

    unordered_map<string, int> subGraphStations;
    unordered_map<string, int> superGraphStations;

    unordered_set<string> lines;
    unordered_map<string, Station::StationH> stationsPerLine;

    Graph graph;
    Graph subGraph;
    Graph superGraph;

};


#endif //DA_SUPERVISOR_H
