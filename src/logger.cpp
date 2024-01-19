#include "logger.hpp"

std::map<logger::STREAM, std::ostream*> logger::stream {
	{ std_out, &std::cout },
	{ std_err, &std::cerr }
};

std::ostream *logger::file_stream = nullptr;

bool logger::silence = false;
bool logger::use_timestamps = false;
std::string logger::prefix;
size_t logger::max_log_entries = 100;
uint8_t logger::log_level = 2; // default to info level

logger::entry logger::_private::last;
std::vector<logger::entry> logger::_private::store;
std::mutex logger::_private::m;

logger::LOG_LEVEL logger::error("error", 0, std_err);
logger::LOG_LEVEL logger::warning("warning", 1);
logger::LOG_LEVEL logger::notice("notice", 2);
logger::LOG_LEVEL logger::info("info", 3);
logger::LOG_LEVEL logger::verbose("verbose", 4);
logger::LOG_LEVEL logger::vverbose("verbose", 5);
logger::LOG_LEVEL logger::debug("debug", 255);

const logger::entry logger::last() {

	std::vector<logger::entry> h = logger::history(1);
	return h.empty() ? logger::entry() : h.back();
}

const logger::entry logger::last(const logger::LOG_LEVEL& level) {

	std::vector<logger::entry> h = logger::history(1, level);
	return h.empty() ? logger::entry() : h.back();
}

const std::vector<logger::entry> logger::history(size_t count) {

	std::vector<logger::entry> _store;

	if ( logger::_private::store.empty())
		return _store;

	while ( logger::_private::store.size() >= logger::max_log_entries + 1 )
		logger::_private::store.erase(logger::_private::store.begin());

	size_t _count = count < 1 || logger::_private::store.size() < count ?
		logger::_private::store.size() : count;

	auto it = _count == logger::_private::store.size() ?
		logger::_private::store.begin() :
			std::next(logger::_private::store.begin(),
				logger::_private::store.size() - _count);

	for ( auto itl = it; itl != logger::_private::store.end(); ++itl )
		_store.push_back(*itl);

	return _store;
}

const std::vector<logger::entry> logger::history(size_t count, const logger::LOG_LEVEL& level) {

	std::vector<logger::entry> _filtered;

	while ( logger::_private::store.size() >= logger::max_log_entries + 1 )
		logger::_private::store.erase(logger::_private::store.begin());

	for ( auto it = logger::_private::store.begin(); it != logger::_private::store.end(); ++it ) {
		if ( it -> id == level.id())
			_filtered.push_back(*(it));
	}

	std::vector<logger::entry> _store;

	if ( _filtered.empty())
		return _store;

	size_t _count = count < 1 || _filtered.size() < count ?
		_filtered.size() : count;

	auto it = _count == _filtered.size() ?
		_filtered.begin() :
			std::next(_filtered.begin(),
				_filtered.size() - _count);

	for ( auto itl = it; itl != _filtered.end(); ++itl )
		_store.push_back(*itl);

	return _store;
}

void logger::loglevel(const logger::LOG_LEVEL& level) {

	logger::log_level = level.id();
}
