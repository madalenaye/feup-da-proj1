#include "Station.h"

Station::Station(std::string name) {
    this->name = name;
}

Station::Station(std::string name,std::string district,std::string municipality,std::string township,std::string line) {
    this->name = name;
    this->district = district;
    this->municipality = municipality;
    this->township = township;
    this->line = line;
}

std::string Station::getName() const {
    return this->name;
}
std::string Station::getDistrict() const {
    return this->district;
}
std::string Station::getLine() const {
    return this->line;
}
std::string Station::getTownship() const {
    return this->township;
}
std::string Station::getMunicipality() const {
    return this->municipality;
}