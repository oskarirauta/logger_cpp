#include "logger/quiet.hpp"

bool logger::quiet::operator ==(const logger::quiet& other) {

	return this -> state == other.state;
}

bool logger::quiet::operator ==(const logger::quiet& other) const {

        return this -> state == other.state;
}

logger::quiet::operator std::string() const {

        return this -> state ? "true" : "false";
}

std::ostream& logger::operator <<(std::ostream& os, const logger::quiet& d) {

	os << char(002) << "quiet:" << d.operator std::string() << char(003);
	return os;
}
