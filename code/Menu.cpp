#include "Menu.h"

Menu::Menu() {
    printf("\n");
    printf("\033[44m===========================================================\033[0m\t\t");
    std::cout << "\n\n" << " Welcome!\n (Press [0] whenever you wish to go back)\n\n";
    supervisor = new Supervisor();
}

void Menu::init() {
    std::string option;
    while(true) {
        std::cout << " What would you like to do today?\n\n"
                " [1] Explore basic service metrics\n"
                " [2] Consult operation cost optimization\n"
                " [3] Analyse reliability and sensitivity to line failures\n"
                " [4] Exit\n\n Option: ";

        std::cin >> option;

        if (option == "1"){
            basicService();
        }

        else if (option == "2"){
            std::cin.ignore();
            std::string source, target;
            if((validatePath(source,target) == "0")) {
                std::cout << "\n";
                continue;
            }
            costOptimization(false, source, target);
        }

        else if (option == "3"){
            reliability();
        }
        else if (option == "4") {
            return;
        }
        else if (option == "0")
            std::cout << "\n You can't go back any further!\n\n";
        else{
            std::cout << "\n Invalid input, try again. \n\n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

void Menu::basicService(){
    std::string option;
    while(true){
        std::cout << "\n What details in specific?\n\n"
                " [1] Maximum number of trains that can simultaneously travel between two specific stations\n"
                " [2] Which pairs of stations require the most amount of trains\n"
                " [3] Top-k municipalities or districts\n"
                " [4] Maximum number of trains that can simultaneously arrive at a given station\n\n"
                " Option: ";

        std::cin >> option;
        if (option == "1"){
            std::cin.ignore();
            std::string source, target;
            if((validatePath(source,target) == "0")) {
                continue;
            }
            maxFlow(false, source,target);
            return;
        }
        else if (option == "2"){
            t2();
        }
        else if (option == "3"){
            statistics();
        }
        else if (option == "4"){
            std::cin.ignore();
            std::string station = validateStation("\n Insert the name of the station (ex: Rio Tinto): ");
            if (station == "0") {
                continue;
            }
            maxStationFlow(station);
            return;
        }
        else if (option == "0"){
            std::cout << "\n";
            return;
        }
        else{
            std::cout << "\n Invalid input, try again. \n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

//2.1
void Menu::maxFlow(bool subgraph, const std::string& srcStation, const std::string& destStation) {

    int src, dest;
    Graph graph = subgraph ? supervisor->getSubGraph() : supervisor->getGraph();
    std::unordered_map<std::string,int> idStations = subgraph ? supervisor->getSubGraphStations() : supervisor->getId();

    src = idStations[srcStation];
    dest = idStations[destStation];
    int maxFlow = graph.maxFlow(src,dest);

    std::cout << "\n Maximum number of trains between " << "\033[1m\033[36m" << srcStation << "\033[0m"
    << " and " << "\033[1m\033[36m" << destStation << "\033[0m" << ": "
    << "\033[1m\033[42m" << " " << maxFlow * 2 << " " << "\033[0m" << "\n\n";

}

//standby
void Menu::t2() {
    std::list<std::pair<std::string, std::string>> pairs;
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
    std::cout << "Max: " << max << "\n"; //....
    for (const auto& pair: pairs){
        std::cout << pair.first <<" - " << pair.second << "\n";
    }

}

//2.3
void Menu::statistics(){
    std::string option;
    while(true) {
        std::cout << "\n Municipalities or districts?\n\n"
                " [1] Municipalities\n"
                " [2] Districts\n\n"
                " Option: ";
        std::cin >> option;
        if (option == "1") {
            transportNeeds(1);
        } else if (option == "2") {
            transportNeeds(0);
        } else if (option == "0") {
            return;
        } else {
            std::cout << "\n Invalid input, try again. \n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}
void Menu::transportNeeds(int type){
    std::string option;
    while(true) {
        std::cout << "\n What would you like to know?\n\n";
        if (type){
            std::cout << " [1] Transport needs inside a municipality\n"
                " [2] Transport needs with other municipalities\n\n"
                " Option: ";
        }
        else {
            std::cout << " [1] Transport needs inside a district\n"
                    " [2] Transport needs with other districts\n\n"
                    " Option: ";
        }
        std::cin >> option;
        if (option == "1") {
            auto result = supervisor->maxConnectedStations(type);
            int choice = showTop(), top;
            if (choice == 1) top = 10;
            else if (choice == 2) top = 15;
            else if (choice == 3) top = customTop("\n Pick a custom top: ", result.size());
            else continue;

            for (int i = 0; i < top; i++)
                std::cout << "\n\033[1m\033[36m " << i+1 << ".\033[0m "<< result[i].first << " | Max nº of connected stations: "
                     << "\033[1m\033[32m" << result[i].second << "\033[0m \n";

        } else if (option == "2") {
            auto result = supervisor->transportNeeds(type);
            int choice = showTop(), top;
            if (choice == 1) top = 10;
            else if (choice == 2) top = 15;
            else if (choice == 3) top = customTop("\n Pick a custom top: ", result.size());
            else continue;

            for (int i = 0; i < top; i++)
                std::cout << "\n\033[1m\033[32m " << i+1 << ".\033[0m "<< result[i].first << " | Maximum flow: "
                << "\033[1m\033[35m" << result[i].second << "\033[0m \n";

        } else if (option == "0") {
            return;
        } else {
            std::cout << "\n Invalid input, try again. \n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

//2.4
void Menu::maxStationFlow(const std::string& station){
    int maxFlow = supervisor->maxStationFlow(supervisor->getId()[station]);
    std::cout << "\n Maximum number of trains that can \033[1m\033[36msimultaneously\033[0m arrive at "
    << "\033[1m\033[43m " << station << " \033[0m" << " : "
    << "\033[1m\033[35m" << maxFlow * 2 << "\033[0m \n" << "\n";
    return;
}

//4.2
void Menu::costOptimization(bool subgraph, const std::string& srcStation, const std::string& destStation){

    int src, dest;
    Graph graph = subgraph ? supervisor->getSubGraph() : supervisor->getGraph();
    std::unordered_map<std::string,int> idStations = subgraph ? supervisor->getSubGraphStations() : supervisor->getId();

    src = idStations[srcStation];
    dest = idStations[destStation];

    int cost = graph.minCost(src,dest);
    std::cout << "\n\033[1m\033[36m Minimum\033[0m cost for the \033[1m\033[34mmaximum\033[0m amount of trains between "
            "\033[1m\033[45m " << srcStation << " \033[0m and \033[1m\033[43m " << destStation << " \033[0m : "
            << "\033[1m\033[36m" << cost << "\033[0m\n\n";

}

//4.3
void Menu::reliability(){
    std::string option;
    while(true) {
        std::cout << "\n What aspect of the railway network you would like to assess the sensitivity of to failures?\n\n"
                " [1] Line failures\n"
                " [2] Segment failures\n"
                " [3] Station failures\n\n"
                " Option: ";

        std::cin >> option;
        if (option == "1") {
            lineFailures();
        }
        else if (option == "2") {
            segmentFailures();
        }
        else if (option == "3"){
            stationFailures();
        }
        else if (option == "0"){
            std::cout << "\n";
            return;
        }
        else{
            std::cout << "\n Invalid input, try again. \n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

void Menu::lineFailures() {

    std::unordered_set<std::string> failedLines;
    std::string option;

    std::string line = validateLine();
    if (line == "0") return;
    failedLines.insert(line);

    while(true){
        std::cout << "\n Would you like to remove any other line?\n\n"
                " [1] Yes\n"
                " [2] No\n\n"
                " Option: ";
        std::cin >> option;
        if (option == "1") {
            line = validateLine();
            if (line == "0") break;
            while (!failedLines.insert(line).second){
                std::cout << " This line has already been chosen, please choose a different line!\n";
                line = validateLine();
                if (line == "0") break;
            }
            failedLines.insert(line);
        }
        else if (option == "2")
            break;
        else if (option == "0")
            return;
        else{
            std::cout << "\n Invalid input, try again. \n\n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
    supervisor->createSubgraph(failedLines);
    subGraphOperations();
}

void Menu::segmentFailures(){

    std::vector<std::pair<std::string,std::string>> failedSegments;
    std::string option;

    std::cin.ignore();
    std::string source, target;
    if ((validatePath(source,target)) == "0") return;

    failedSegments.emplace_back(source,target);

    while(true){
        std::cout << "\n Would you like to remove any other segment?\n\n"
                " [1] Yes\n"
                " [2] No\n\n"
                " Option: ";
        std::cin >> option;
        if (option == "1"){
            std::cin.ignore();
            validatePath(source,target);

            while (std::any_of(failedSegments.begin(), failedSegments.end(),
                               [&](std::pair<std::string,std::string>& segment){ return segment == std::make_pair(source, target); })) {
                std::cout << " This segment has already been chosen, please choose a different segment!\n";
                validatePath(source,target);
            }
            if (source == "0" || target == "0") break;
            failedSegments.emplace_back(source,target);
        }
        else if (option == "2")
            break;
        else if (option == "0")
            return;
        else{
            std::cout << "\n Invalid input, try again. \n\n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
    supervisor->createSubgraph(failedSegments);
    subGraphOperations();
}

void Menu::stationFailures(){
    Station::StationH failedStations;
    std::string option;
    std::cin.ignore();
    std::string name = validateStation("\n Insert the name of the station (ex: Rio Tinto): ");
    if (name == "0") return;
    Station station = *supervisor->getStations().find(name);
    failedStations.insert(station);

    while(true){
        std::cout << "\n Would you like to remove any other station?\n\n"
                " [1] Yes\n"
                " [2] No\n\n"
                " Option: ";
        std::cin >> option;
        if (option == "1"){
            std::cin.ignore();
            name = validateStation(" Insert the name of the station (ex: Rio Tinto): ");
            if (name == "0") break;
            station = *supervisor->getStations().find(name);
            while (!failedStations.insert(station).second){
                std::cout << " This station has already been chosen, please choose a different station!\n";
                name = validateStation(" Insert the name of the station (ex: Rio Tinto): ");
                if (name == "0") break;
                station = *supervisor->getStations().find(name);
            }
            failedStations.insert(station);
        }
        else if (option == "2")
            break;
        else if (option == "0")
            return;
        else{
            std::cout << "\n Invalid input, try again. \n\n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
    supervisor->createSubgraph(failedStations);
    subGraphOperations();
}

void Menu::subGraphOperations(){
    std::string option;
    while(true){
        std::cout << "\n What type of information would you like to see?\n\n"
                " [1] Maximum number of trains that can simultaneously travel between two specific stations\n"
                " [2] Minimum cost for the maximum flow between two specific stations\n"
                " [3] Top-k most affected stations\n\n"
                " Option: ";

        std::cin >> option;
        if (option == "1"){
            std::cin.ignore();
            std::string source, target;
            validatePath(source,target);

            maxFlow(true,source,target);
            return;
        }
        else if (option == "2"){
            std::cin.ignore();
            std::string source, target;
            validatePath(source,target);

            costOptimization(true,source,target);
            return;
        }
        else if (option == "3"){
            mostAffectedStations();
            return;
        }
        else if (option == "0")
            return;
        else{
            std::cout << "\n Invalid input, try again. \n\n";
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
}

void Menu::mostAffectedStations(){
    std::vector<std::pair<std::string,int>> difference = supervisor->flowDifference();
}


//validate
std::string Menu::validateLine() {
    std::string line;

    std::cout << "\n Insert the name of the line you would like to remove (ex: Linha do Minho): ";
    std::cin.ignore();
    getline(std::cin,line,'\n');

    while(!supervisor->isLine(line)) {
        if (line == "0") return "0";
        else std::cout << " That line does not exist in our database " << '\n';
        std::cout << " Insert the name of a valid line (ex: Linha do Minho): ";
        std::cin.clear();
        getline(std::cin,line,'\n');
    }
    return line;
}

std::string Menu::validateStation(const std::string& message){

    std::string station;

    std::cout << message;

    getline(std::cin,station,'\n');

    while(!supervisor->isStation(station)) {
        if (station == "0") return "0";
        else std::cout << " That station does not exist in our database " << '\n';
        std::cout << " Insert the name of a valid station (ex: Lisboa Oriente): ";
        std::cin.clear();
        getline(std::cin,station,'\n');
    }
    return station;
}

std::string Menu::validatePath(std::string& source, std::string& target){
    source = validateStation("\n Insert the name of the source station (ex: Porto Campanhã): ");
    if (source == "0") return "0";
    target = validateStation(" Insert the name of the target station (ex: Lisboa Oriente): ");
    if (target == "0") return "0";

    while (source == target){
        std::cout << " Please choose two different stations.\n";
        target = validateStation(" Insert the name of the target station (ex: Lisboa Oriente): ");
    }
    if (target == "0") return "0";

    return "";
}
//top-k
int Menu::showTop(){
    std::cout << "\n What do you wish to check:\n\n "
            "[1] Top 10\n [2] Top 15\n [3] Other\n\n Option: ";
    int option;
    std::cin >> option;
    while (std::cin.fail() || option < 0  || option > 4){
        std::cout << " Invalid input\n Try again: ";
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        std::cin >> option;
    }
    return option;
}
int Menu::customTop(const std::string& message, unsigned int n) {
    std::cout << message;
    int option; std::cin >> option;
    while (std::cin.fail() || option < 0 || option > n){
        std::cout << " Choose a number between 1 and " << n << "\n Try again: ";
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        std::cin >> option;
    }
    return option;
}


//end
void Menu::end() {
    printf("\n");
    printf("\033[46m===========================================================\033[0m\n");
}

