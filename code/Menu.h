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
    //T2
    void t2(); //T2.2

    void statistics(); //T2.3
    void t4(const string& destStation); //T2.4

    void costOptimization(bool subgraph, const string& srcStation, const string& destStation);

    //validate

    int customTop(const string& message, int n);


    void reliability();

    void lineFailures();
    void segmentFailures();
    void stationFailures();

    string validateLine();
    string validateStation(bool dest);
    void subGraphOperations();

    Supervisor* supervisor;
};
#endif //DA_MENU_H
