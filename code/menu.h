//
// Created by Madalena Ye on 11/03/2023.
//
#ifndef DA_MENU_H
#define DA_MENU_H

#include "supervisor.h"
#include "station.h"
#include <iostream>

class Menu{
public:
    Menu();
    void init();
    static void end();

private:
    Supervisor* supervisor;
};
#endif //DA_MENU_H
