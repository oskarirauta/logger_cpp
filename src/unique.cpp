#include "logger/unique.hpp"

bool logger::unique::operator ==(const logger::unique& other) {

	return this -> state == other.state;
}

bool logger::unique::operator ==(const logger::unique& other) const {

        return this -> state == other.state;
}

logger::unique::operator std::string() const {

        return this -> state ? "true" : "false";
}

std::ostream& logger::operator <<(std::ostream& os, const logger::unique& d) {

	os << char(002) << "unique:" << d.operator std::string() << char(003);
	return os;
}
