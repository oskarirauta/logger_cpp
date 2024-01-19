#include "logger/detail.hpp"

bool logger::detail::operator ==(const logger::detail& other) {

        return this -> txt == other.txt;
}

bool logger::detail::operator ==(const logger::detail& other) const {

        return this -> txt == other.txt;
}

logger::detail::operator std::string() const {

        return this -> txt;
}

std::ostream& logger::operator <<(std::ostream& os, const logger::detail& d) {

	os << char(002) << "detail:" << d.operator std::string() << char(003);
	return os;
}
