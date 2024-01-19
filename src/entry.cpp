#include <utility>
#include <ctime>

#include "common.hpp"
#include "logger.hpp"

bool logger::entry::has_detail() const {

	return !this -> detail.empty();
}

bool logger::entry::has_tag() const {

	return !this -> tag.empty();
}

bool logger::entry::operator ==(const logger::entry& other) {

	return this -> id == other.id &&
		common::to_lower(std::as_const(this -> msg)) == common::to_lower(std::as_const(other.msg));
}

bool logger::entry::operator ==(const logger::entry& other) const {

	return this -> id == other.id &&
		common::to_lower(std::as_const(this -> msg)) == common::to_lower(std::as_const(other.msg));
}

logger::entry::operator std::string() const {

	return this -> detail_title() + this -> msg;
}

const std::string logger::entry::get_timestamp() const {

	auto ts = this -> timestamp_last.count();
	std::string s = common::trim_ws(ctime(&ts));
	return s;
}

const std::string logger::entry::detail_title() const {

	return
		( !logger::use_timestamps ? "" : ( this -> get_timestamp() + " " )) +
		( !logger::prefix.empty() ? ( logger::prefix + "::" ) : "" ) + this -> name +
                ( this -> has_tag() ? ( " [" + this -> tag + "]: " ) : ": " );
}

const std::string logger::entry::detail_spacing() const {

	std::string s = this -> detail_title();
	for ( auto& ch : s )
		ch = ' ';
	return s;
}

std::ostream& logger::operator <<(std::ostream& os, const logger::entry& e) {

	os << e.operator std::string();
        return os;
}
