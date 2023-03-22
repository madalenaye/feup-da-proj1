//
// Created by Madalena Ye on 11/03/2023.
//
#ifndef DA_MENU_H
#define DA_MENU_H

#include "supervisor.h"
#include "station.h"
#include <iostream>
#include <climits>

class Menu{
public:
    Menu();
    void init();
    static void end();

private:
    void basicService();

    //T2
    void maxFlow(const string& srcStation, const string& destStation); //T2.1
    void t2(); //T2.2
    void statistics(); //T2.3
    void t4(const string& destStation); //T2.4

    //T3
    void costOptimization(const string& srcStation, const string& destStation);

    //T4
    void lineFailures();

    //validate

    int customTop(const string& message, int n);
    string validateStation(bool dest);


    Supervisor* supervisor;
};
#endif //DA_MENU_H
