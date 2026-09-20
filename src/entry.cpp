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

bool logger::entry::operator ==(const logger::entry& other) const {

	return this -> id == other.id &&
		common::to_lower(std::as_const(this -> msg)) == common::to_lower(std::as_const(other.msg));
}

logger::entry::operator std::string() const {

	return this -> detail_title() + this -> msg;
}

const std::string logger::entry::get_timestamp() const {

	time_t ts = static_cast<time_t>(this -> timestamp_last.count());
	char buf[32] = {0};
#if defined(_WIN32)
	ctime_s(buf, sizeof(buf), &ts);
#else
	ctime_r(&ts, buf);
#endif
	return common::trim_ws(buf);
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
