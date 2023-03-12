//
// Created by Madalena Ye on 11/03/2023.
//

#ifndef DA_STATION_H
#define DA_STATION_H

#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class Station{
public:
    explicit Station(string name);
    Station(string, string, string, string, string);
    string getName() const;
    string getDistrict() const;
    string getMunicipality() const;
    string getTownship() const;
    string getLine() const;
    struct StationHash{
        int operator()(const Station& station){
            string c=station.getName();
            int v=0;
            for(char i:c){
                v=37*v+i;
            }
            return v;
        }
        bool operator()(const Station& station1,const Station& station2){
                return station1.getName()==station2.getName();
        }
    };
    typedef unordered_set<Station,Station::StationHash,Station::StationHash> StationH;
private:
    string name;
    string district;
    string municipality;
    string township;
    string line;
};

#endif //DA_STATION_H
