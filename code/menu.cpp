//
// Created by Madalena Ye on 11/03/2023.
//

#include <list>
#include "menu.h"

Menu::Menu() {
    printf("\n");
    printf("\033[44m===========================================================\033[0m\t\t");
    cout << "\n\n" << " Welcome!\n (Press [0] whenever you wish to go back)\n\n";
    supervisor = new Supervisor();
}

void Menu::init() {
    string option;
    while(true) {
        cout << " What would you like to do today?\n\n"
                " [1] Explore basic service metrics\n [2] Consult operation cost optimization\n [3] Analyse reliability and sensitivity to line failures\n"
                " [4] Exit\n\n Option: ";

        cin >> option;

        if (option == "1"){
            basicService();
        }

        else if (option == "2"){
            string srcStation = validateStation(false);
            string destStation = validateStation(true);

            while (destStation == srcStation){
                cout << "Please choose two different stations.\n";
                destStation = validateStation(true);
            }
            costOptimization(srcStation, destStation);
        }

        else if (option == "3"){

        }
        else if (option == "4")
            return;
        else if (option == "0")
            cout << "\n You can't go back any further!\n\n";

        else{
            cout << "\n Invalid input, try again. \n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::basicService(){
    string option;
    while(true){
        cout << "\n What details in specific?\n\n"
                " [1] Maximum number of trains that can simultaneously travel between two specific stations\n"
                " [2] Which pairs of stations require the most amount of trains\n"
                " [3] Top-k municipalities or districts\n [4] Maximum number of trains that can simultaneously arrive at a given station\n\n "
                " Option: ";

        cin >> option;
        if (option == "1"){
            string srcStation = validateStation(false);
            string destStation = validateStation(true);

            while (destStation == srcStation){
                cout << "Please choose two different stations.\n";
                destStation = validateStation(true);
            }

            maxFlow(srcStation,destStation);
            return;
        }
        else if (option == "2"){
            t2(); //....
            return;
        }
        else if (option == "3"){
//            statistics();
            return;
        }
        else if (option == "4"){
            string destStation = validateStation(true);
            t4(destStation); //....
            return;
        }
        else if (option == "0"){
            cout << "\n";
            return;
        }
        else{
            cout << "\n Invalid input, try again. \n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

void Menu::maxFlow(const string& srcStation, const string& destStation) {
    int src = supervisor->getId()[srcStation];
    int dest = supervisor->getId()[destStation];

    int maxFlow = supervisor->getGraph().maxFlow(src,dest);
    cout << "Maximum number of trains between " << srcStation << " and " << destStation << " : " << maxFlow << "\n";

}

void Menu::t2() {
    list<pair<string, string>> pairs;
    int max = 0;

    for (int i = 1; i < supervisor->getGraph().getVertexSet().size(); i++){
        for (int j = i+1; j < supervisor->getGraph().getVertexSet().size(); j++){
            int flow = supervisor->getGraph().maxFlow(i, j);
            if (max < flow) {
                pairs.clear();
                max = flow;
            }
            if (max == flow)
                pairs.emplace_back(supervisor->getGraph().getVertexSet()[i]->getStation().getName(),supervisor->getGraph().getVertexSet()[j]->getStation().getName());
            }
        }
    cout << "Max: " << max << "\n"; //....
    for (const auto& pair: pairs){
        cout << pair.first <<" - " << pair.second << "\n";
    }

}

void Menu::t4(const string& destStation){
    list<pair<string, string>> pairs;
    int dest = supervisor->getId()[destStation];
    int max = 0;
    for (int i = 1; i < supervisor->getGraph().getVertexSet().size(); i++){
        if (i == dest) continue;
        int flow = supervisor->getGraph().maxFlow(i, dest);
        if (max < flow) {
        pairs.clear();
        max = flow;
        }
        if (max == flow)
        pairs.emplace_back(supervisor->getGraph().getVertexSet()[i]->getStation().getName(),destStation);
    }
    cout << "Max: " << max << "\n"; //....
    for (const auto& pair: pairs){
        cout << pair.first <<" - " << pair.second << "\n";
    }
}

void Menu::costOptimization(const string& srcStation, const string& destStation){
    int src = supervisor->getId()[srcStation];
    int dest = supervisor->getId()[destStation];

    int cost = supervisor->getGraph().minCost(src,dest);
    cout << "Minimum cost for the maximum amount of trains between " << srcStation << " and " << destStation << " : " << cost << "\n";

}
void Menu::lineFailures(){

}
string Menu::validateStation(bool dest){

    string station;

    if (dest)
        cout << " Insert the name of the target station (ex: Porto Campanhã): ";
    else{
        cin.ignore();
        cout << " Insert the name of the source station (ex: Porto Campanhã): ";
    }
    getline(cin,station,'\n');

    while(!supervisor->isStation(station)) {
        if (station == "0") return "0";
        else cout << " That station does not exist in our database " << '\n';
        cout << " Insert the name of a valid station (ex: Porto Campanhã): ";
        cin.clear();
        getline(cin,station,'\n');
    }
    return station;
}




int Menu::customTop(const string& message, int n) {
    cout << message;
    int option; cin >> option;
    while (cin.fail() || option < 0 || option > n){
        cout << " Choose a number between 1 and " << n << "\n Try again: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> option;
    }
    return option;
}

void Menu::end() {
    printf("\n");
    printf("\033[46m===========================================================\033[0m\n");
}

