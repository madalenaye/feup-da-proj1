
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


class Supervisor{
public:

    Supervisor();

    //getters
    Station::StationH getStations() const;
    std::unordered_map<std::string, int> getId() const;
    Graph getGraph() const;
    Graph getSubGraph() const;
    std::unordered_map<std::string, int> getSubGraphStations() const;

    //validate
    bool isStation(const std::string& station);
    bool isLine(const std::string& line);

    void createSubgraph(const std::unordered_set<std::string>& failedLines);
    void createSubgraph(const std::vector<std::pair<std::string, std::string>>& failedSegments);
    void createSubgraph(const Station::StationH& failedStations);


    std::vector<std::pair<std::string, int>> transportNeeds(bool type);
    std::vector<std::pair<std::string, int>> maxConnectedStations(int type);

    int maxStationFlow(int target);

    std::vector<std::pair<std::string, int>> flowDifference();

private:

    void createStations();
    void createGraph();
    void createSuperSource(int id, Station::StationH targetStations);
    void createSuperSink(int id, Station::StationH targetStations);
    void createSuperSourceGraph(int target);
    void createSuperGraph(const Station::StationH& targetStations);


    static void checkField(std::istringstream &iss, std::string &field);
    static std::string removeQuotes(std::istringstream &iss, std::string field);

    int makeVertex(Graph &_graph, std::unordered_map<std::string, int> &ids, const std::string &name, int &id);

    static bool segmentFailure(const std::vector<std::pair<std::string, std::string>>& failedSegments, const std::string& source, const std::string& target);
    bool lineFailure(std::unordered_set<std::string> failedLines, const std::string &source, const std::string &target);
    static bool stationFailure(Station::StationH failedStations, const std::string &source, const std::string &target);

    Station::StationH stations;
    std::unordered_map<std::string, int> idStations;

    std::unordered_map<std::string, Station::StationH> municipalityStations;
    std::unordered_map<std::string, Station::StationH> districtStations;

    std::unordered_map<std::string, int> subGraphStations;
    std::unordered_map<std::string, int> superGraphStations;

    std::unordered_set<std::string> lines;
    std::unordered_map<std::string, Station::StationH> stationsPerLine;

    Graph graph;
    Graph subGraph;
    Graph superGraph;

};


#endif //DA_SUPERVISOR_H
