#ifndef DA_MENU_H
#define DA_MENU_H

#include <iostream>
#include <climits>
#include <list>
#include <atomic>
#include <thread>

#include "Supervisor.h"

class Menu {
public:

    Menu();
    void init();
    static void end();

private:

    Supervisor* supervisor;

    void basicService();

    void maxFlow(bool subgraph, const std::string &srcStation, const std::string &destStation);

    void mostAmountOfTrains();

    void statistics();
    void transportNeeds(int type);

    void maxStationFlow(const std::string& station);

    void costOptimization(bool subgraph, const std::string& srcStation, const std::string& destStation);

    void reliability();

    void lineFailures();
    void segmentFailures();
    void stationFailures();

    void subGraphOperations();
    void mostAffectedStations();

    static int customTop(const std::string& message, unsigned int n);
    static int showTop();

    std::string validateLine();
    std::string validateStation(const std::string& message);
    std::string validatePath(std::string &source, std::string &target);

};
#endif //DA_MENU_H
