#include "station.h"

Station::Station(string name) {
    this->name = name;
}

Station::Station(string name,string district,string municipality,string township,string line) {
    this->name = name;
    this->district = district;
    this->municipality = municipality;
    this->township = township;
    this->line = line;
}

string Station::getName() const {
    return this->name;
}
string Station::getDistrict() const {
    return this->district;
}
string Station::getLine() const {
    return this->line;
}
string Station::getTownship() const {
    return this->township;
}
string Station::getMunicipality() const {
    return this->municipality;
}