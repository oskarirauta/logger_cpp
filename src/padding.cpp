#include "logger/padding.hpp"

bool logger::padding::operator ==(const logger::padding& other) {

        return this -> count == other.count && this -> width == other.width;
}

bool logger::padding::operator ==(const logger::padding& other) const {

        return this -> count == other.count && this -> width == other.width;
}

logger::padding::operator std::string() const {

	return std::to_string(this -> count) + "," + std::to_string(this -> width);
}

bool logger::padding::empty() {

	return this -> count == 0 || this -> width == 0;
}

bool logger::padding::empty() const {

	return this -> count == 0 || this -> width == 0;
}

void logger::padding::clear() {

	this -> count = 0;
	this -> width = 0;
}

std::ostream& logger::operator <<(std::ostream& os, const logger::padding& p) {

	os << char(002) << "padding:" << p.operator std::string() << char(003);
	return os;
}
