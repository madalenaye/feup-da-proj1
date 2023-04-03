#include "Menu.h"

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
            costOptimization(false, srcStation, destStation);
        }

        else if (option == "3"){
            reliability();
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

            maxFlow(false, srcStation,destStation);
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

void Menu::maxFlow(bool subgraph, const string& srcStation, const string& destStation) {

    int src, dest;
    Graph graph = subgraph ? supervisor->getSubGraph() : supervisor->getGraph();
    unordered_map<string,int> idStations = subgraph ? supervisor->getSubGraphStations() : supervisor->getId();

    src = idStations[srcStation];
    dest = idStations[destStation];
    int maxFlow = graph.maxFlow(src,dest);

    cout << "Maximum number of trains between " << srcStation << " and " << destStation << " : " << maxFlow << "\n";

}

void Menu::t2() {
    list<pair<string, string>> pairs;
    int max = 0;

    for (int i = 0; i < supervisor->getGraph().getVertexSet().size(); i++){
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
    for (int i = 0; i < supervisor->getGraph().getVertexSet().size(); i++){
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

void Menu::costOptimization(bool subgraph, const string& srcStation, const string& destStation){

    int src, dest;
    Graph graph = subgraph ? supervisor->getSubGraph() : supervisor->getGraph();
    unordered_map<string,int> idStations = subgraph ? supervisor->getSubGraphStations() : supervisor->getId();

    src = idStations[srcStation];
    dest = idStations[destStation];

    int cost = graph.minCost(src,dest);
    cout << " Minimum cost for the maximum amount of trains between " << srcStation << " and " << destStation << " : " << cost << "\n";

}

void Menu::reliability(){
    string option;
    while(true) {
        cout << "\n What aspect of the railway network you would like to assess the sensitivity of to failures?\n\n"
                " [1] Line failures\n"
                " [2] Segment failures\n"
                " [3] Station failures\n\n"
                " Option: ";

        cin >> option;
        if (option == "1") {
            lineFailures();
            return;
        }
        else if (option == "2") {
            segmentFailures();
            return;
        }
        else if (option == "3"){
            stationFailures();
            return;
        }
        else if (option == "0"){
            return;
        }
        else{
            cout << "\n Invalid input, try again. \n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

void Menu::lineFailures() {

    unordered_set<string> failureLines;
    string option;

    string line = validateLine();
    if (line == "0") return;
    failureLines.insert(line);

    while(true){
        cout << "\n Would you like to remove any other line?\n\n"
                " [1] Yes\n"
                " [2] No\n\n "
                " Option: ";
        cin >> option;
        if (option == "1") {
            line = validateLine();
            if (line == "0") break;
            while (!failureLines.insert(line).second){
                cout << " This line has already been chosen, please choose a different line!\n";
                line = validateLine();
                if (line == "0") break;
            }
            failureLines.insert(line);
        }
        else if (option == "2")
            break;
        else if (option == "0")
            return;
        else{
            cout << "\n Invalid input, try again. \n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
    supervisor->createSubgraph(failureLines);
    subGraphOperations();
}

void Menu::segmentFailures(){
    vector<pair<string,string>> failureSegments;
    string option;

    string source = validateStation(false);
    if (source == "0") return;
    string target = validateStation(true);
    if (target == "0") return;

    while (source == target){
        cout << "Please choose two different stations.\n";
        target = validateStation(true);
    }
    if (target == "0") return;
    failureSegments.emplace_back(source,target);

    while(true){
        cout << "\n Would you like to remove any other segment?\n\n"
                " [1] Yes\n"
                " [2] No\n\n "
                " Option: ";
        cin >> option;
        if (option == "1"){
            source = validateStation(false);
            if (source == "0") break;
            target = validateStation(true);
            if (target == "0") break;
            while (source == target){
                cout << "Please choose two different stations.\n";
                target = validateStation(true);
            }
            if (target == "0") break;
            while (std::any_of(failureSegments.begin(), failureSegments.end(),
                               [&](pair<string,string>& segment){ return segment == std::make_pair(source, target); })) {
                cout << " This segment has already been chosen, please choose a different segment!\n";
                source = validateStation(false);
                if (source == "0") break;
                target = validateStation(true);
                if (target == "0") break;

                while (source == target){
                    cout << "Please choose two different stations.\n";
                    target = validateStation(true);
                }
                if (target == "0") break;
            }

            failureSegments.emplace_back(source,target);
        }
        else if (option == "2")
            break;
        else if (option == "0")
            return;
        else{
            cout << "\n Invalid input, try again. \n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
    supervisor->createSubgraph(failureSegments);
    subGraphOperations();
}

void Menu::stationFailures(){
    Station::StationH failureStations;
    string option;

    string name = validateStation(false);
    if (name == "0") return;
    Station station = *supervisor->getStations().find(name);
    failureStations.insert(station);

    while(true){
        cout << "\n Would you like to remove any other station?\n\n"
                " [1] Yes\n"
                " [2] No\n\n "
                " Option: ";
        cin >> option;
        if (option == "1"){
            name = validateStation(false);
            if (name == "0") break;
            station = *supervisor->getStations().find(name);
            while (!failureStations.insert(station).second){
                cout << " This station has already been chosen, please choose a different station!\n";
                name = validateStation(false);
                if (name == "0") break;
                station = *supervisor->getStations().find(name);;
            }
            failureStations.insert(station);
        }
        else if (option == "2")
            break;
        else if (option == "0")
            return;
        else{
            cout << "\n Invalid input, try again. \n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
    supervisor->createSubgraph(failureStations);
    subGraphOperations();
}

void Menu::subGraphOperations(){
    string option;
    while(true){
        cout << "\n What type of information would you like to see?\n\n"
                " [1] Maximum number of trains that can simultaneously travel between two specific stations\n"
                " [2] Minimum cost for the maximum flow between two specific stations\n"
                " [3] Top-k most affected stations\n\n "
                " Option: ";

        cin >> option;
        if (option == "1"){
            string srcStation = validateStation(false);
            string destStation = validateStation(true);

            while (destStation == srcStation){
                cout << "Please choose two different stations.\n";
                destStation = validateStation(true);
            }

            maxFlow(true,srcStation,destStation);
            return;
        }
        else if (option == "2"){
            string srcStation = validateStation(false);
            string destStation = validateStation(true);

            while (destStation == srcStation){
                cout << "Please choose two different stations.\n";
                destStation = validateStation(true);
            }

            costOptimization(true,srcStation,destStation);
            return;
        }
        else if (option == "3"){
            //to be implemented...
            return;
        }
        else if (option == "0")
            return;
        else{
            cout << "\n Invalid input, try again. \n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

string Menu::validateLine() {
    string line;

    cout << " Insert the name of the line you would like to remove (ex: Linha do Minho): ";
    cin.ignore();
    getline(cin,line,'\n');

    while(!supervisor->isLine(line)) {
        if (line == "0") return "0";
        else cout << " That line does not exist in our database " << '\n';
        cout << " Insert the name of a valid line (ex: Linha do Minho): ";
        cin.clear();
        getline(cin,line,'\n');
    }
    return line;
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

