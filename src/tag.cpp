#include "logger/tag.hpp"

bool logger::tag::operator ==(const logger::tag& other) {

        return this -> name == other.name;
}

bool logger::tag::operator ==(const logger::tag& other) const {

        return this -> name == other.name;
}

logger::tag::operator std::string() const {

        return this -> name;
}

std::ostream& logger::operator <<(std::ostream& os, const logger::tag& t) {

	os << char(002) << "tag:" << t.operator std::string() << char(003);
	return os;
}
