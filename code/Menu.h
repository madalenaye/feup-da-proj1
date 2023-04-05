#ifndef DA_MENU_H
#define DA_MENU_H

#include <iostream>
#include <climits>
#include <list>

#include "Supervisor.h"
#include "Station.h"

class Menu{
public:

    Menu();
    void init();
    static void end();

private:

    void basicService();

    void maxFlow(bool subgraph, const string &srcStation, const string &destStation);

    void t2();

    void statistics();
    void transportNeeds(int type);

    void maxStationFlow(const string& station);

    void costOptimization(bool subgraph, const string& srcStation, const string& destStation);

    void reliability();

    void lineFailures();
    void segmentFailures();
    void stationFailures();

    void subGraphOperations();
    void mostAffectedStations();

    static int customTop(const string& message, int n);
    static int showTop();

    string validateLine();
    string validateStation(const string& message);
    string validatePath(string &source, string &target);

    Supervisor* supervisor;

};
#endif //DA_MENU_H
