#ifndef DA_STATION_H
#define DA_STATION_H

#include <string>
#include <unordered_map>
#include <unordered_set>


class Station{
public:
    Station(std::string name);
    Station(std::string, std::string, std::string, std::string, std::string);
    std::string getName() const;
    std::string getDistrict() const;
    std::string getMunicipality() const;
    std::string getTownship() const;
    std::string getLine() const;
    struct StationHash{
        int operator()(const Station& station) const{
            std::string c=station.getName();
            int v=0;
            for(char i:c){
                v=37*v+i;
            }
            return v;
        }
        bool operator()(const Station& station1,const Station& station2) const{
                return station1.getName()==station2.getName();
        }
    };
    typedef std::unordered_set<Station,Station::StationHash,Station::StationHash> StationH;
private:
    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;
};

#endif //DA_STATION_H
