#include "Menu.h"

int main() {
    /*Menu menu;
    menu.init();
    Menu::end();*/

    Supervisor* supervisor = new Supervisor();
    /*vector<pair<string, int>> top = supervisor->topBudget(2);
    for (auto i : top){
        cout << i.first << " : " << i.second << " | ";
    }
    for (auto i : supervisor->getDistrictStations()){
        cout << i.first << " : ";
        for (auto j : i.second){
            cout << j.getName() << " | ";
        }
    }*/
    list<list<Vertex*>> a = supervisor->getGraph().connectedStations("FARO");
    for (auto i : a){
        cout << supervisor->getGraph().kruskal(i) << endl;
    }
}