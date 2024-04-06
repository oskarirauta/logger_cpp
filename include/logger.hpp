#pragma once

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <mutex>
#include <map>

#include "common.hpp"
#include "logger/entry.hpp"
#include "logger/tag.hpp"
#include "logger/detail.hpp"
#include "logger/padding.hpp"
#include "logger/quiet.hpp"
#include "logger/unique.hpp"

namespace logger {

	enum STREAM { std_out, std_err };

	extern std::map<logger::STREAM, std::ostream*> stream;
	extern std::ostream* file_stream;

	extern bool silence;
	extern bool use_timestamps;
	extern std::string prefix;
	extern size_t max_log_entries;
	extern uint8_t log_level;

	template<typename Ch, typename Traits = std::char_traits<Ch>,
		typename Sequence = std::vector<Ch> >
	struct basic_LOG_LEVEL : std::basic_streambuf<Ch, Traits>, public std::ostream {

		typedef std::basic_streambuf<Ch, Traits> base_type;
		typedef typename base_type::int_type int_type;
		typedef typename base_type::traits_type traits_type;

		private:
			basic_LOG_LEVEL& _flush();
			uint8_t _id;
			std::string _name;
			STREAM _stream;

		protected:

			Sequence _buf;
			std::string _tag;
			std::string _detail;
			logger::padding _padding;
			bool _quiet;
			bool _unique;

			void _reset();
			void _parse();
			int _sync();
			int_type _overflow(int_type ch);

			virtual int sync() override { return this -> _sync(); }

		public:

			virtual int_type overflow(int_type ch) { return this -> _overflow(ch); }
			const std::string name() const;
			uint8_t id() const;
			STREAM stream() const;
			void change_logging_level(const uint8_t new_level);

			basic_LOG_LEVEL& operator [](const std::string& t);
			operator std::string() const;

			basic_LOG_LEVEL(const std::string& name, uint8_t level, logger::STREAM stream = logger::std_out) :
				std::ostream(this), _id(level), _name(name), _stream(stream) {}

			template <typename C>
			friend std::ostream& operator <<(std::ostream& os, basic_LOG_LEVEL<C>& l);
	};

	typedef basic_LOG_LEVEL<char> LOG_LEVEL;

	namespace _private {

		extern logger::entry last;
		extern std::vector<logger::entry> store;
		extern std::mutex m;
	}

	int loglevel();
	void loglevel(const logger::LOG_LEVEL& level);
	const logger::entry last();
	const logger::entry last(const logger::LOG_LEVEL& level);
	const std::vector<logger::entry> history(size_t count = 0);
	const std::vector<logger::entry> history(size_t count, const logger::LOG_LEVEL& level);

	extern LOG_LEVEL error;
	extern LOG_LEVEL warning;
	extern LOG_LEVEL notice;
	extern LOG_LEVEL info;
	extern LOG_LEVEL verbose;
	extern LOG_LEVEL vverbose;
	extern LOG_LEVEL debug;

	template<typename C>
	std::ostream& operator <<(std::ostream& os, basic_LOG_LEVEL<C>& l);

}

template<typename Ch, typename Traits, typename Sequence>
logger::basic_LOG_LEVEL<Ch, Traits, Sequence>& logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::_flush() {

	if ( !this -> _tag.empty()) {
		#if __cplusplus >= 202002L
		std::erase_if(this -> _tag, [](char ch) { return std::string("\r\v").find(ch) != std::string::npos; });
		#else
		this -> _tag.erase(std::remove_if(this -> _tag.begin(), this -> _tag.end(),
			[](char ch) { return std::string("\r\v").find(ch) != std::string::npos; }), this -> _tag.end());
		#endif
		std::replace_if(this -> _tag.begin(), this -> _tag.end(),
			[](char ch) { return std::string("\n\t").find(ch) != std::string::npos; }, ' ');
		this -> _tag = common::trim_ws(this -> _tag);
	}

	if ( !this -> _detail.empty()) {
		#if __cplusplus >= 202002L
		std::erase_if(this -> _tag, [](char ch) { return std::string("\r\v").find(ch) != std::string::npos; });
		#else
		this -> _tag.erase(std::remove_if(this -> _tag.begin(), this -> _tag.end(),
			[](char ch) { return std::string("\r\v").find(ch) != std::string::npos; }), this -> _tag.end());
		#endif
		std::replace_if(this -> _tag.begin(), this -> _tag.end(),
			[](char ch) { return std::string("\n\t").find(ch) != std::string::npos; }, ' ');
		this -> _detail = common::trim_ws(this -> _tag);
	}

	logger::entry e = {
		.name = this -> _name,
		.id = this -> _id,
		.msg = common::trim_ws(std::string(this -> _buf.begin(), this -> _buf.end())),
		.tag = this -> _tag,
		.detail = this -> _detail,
		.count = 1
	};

	if ( e.msg.front() == char(001)) {

		e.msg.erase(0, 1);
		while ( !e.msg.empty() && std::string("\r\v\n\t ").find(e.msg.front()) != std::string::npos )
			e.msg.erase(0, 1);
	}

	if ( !e.msg.empty() && !this -> _padding.empty()) {

		std::string new_msg;
		new_msg += char(001);
		for ( size_t i = 0; i < this -> _padding.width * this -> _padding.count; i++ )
			new_msg += ' ';
		e.msg = new_msg + e.msg;
	}

	if ( e.msg.front() == char(001)) {

		e.msg.erase(0, 1);

		while ( !e.msg.empty() && std::string("\r\v\n\t").find(e.msg.front()) != std::string::npos )
			e.msg.erase(0, 1);
	}

	if ( e.name.empty() || e.msg.empty())
		return *this;

	if ( logger::file_stream != nullptr ) {
		*(logger::file_stream) << e << std::endl;
		if ( !e.detail.empty())
			*(logger::file_stream) << e.detail_spacing() << e.detail << std::endl;
	}

	const std::lock_guard<std::mutex> lock(logger::_private::m);

	if ( !this -> _unique && !logger::_private::store.empty() && logger::_private::last == e ) {

		if ( logger::_private::store.back() == e ) {

			if ( !logger::_private::store.back().has_tag() && !this -> _tag.empty())
				logger::_private::store.back().tag = this -> _tag;

			if ( !logger::_private::store.back().has_detail() && !this -> _detail.empty())
				logger::_private::store.back().detail = this -> _detail;

		}

		if ( logger::_private::last.count == 1 ) {

			#ifdef LOG_DUPLICATE_MSG
			e.msg = LOG_DUPLICATE_MSG;
			#else
			e.msg = "(duplicate message atleast once)";
			#endif

			e.tag = logger::_private::last.tag;
			e.detail = "";

			if ( !logger::silence && !this -> _quiet && logger::log_level >= this -> _id )
				*(logger::stream[this -> _stream]) << e << std::endl;
		}

		logger::_private::store.back().timestamp_last =
			std::chrono::duration_cast<std::chrono::seconds>
				(std::chrono::system_clock::now().time_since_epoch());

		logger::_private::store.back().count++;
		logger::_private::last.count++;
		return *this;
	}

	if ( !logger::silence && !this -> _quiet && logger::log_level >= this -> _id )
		*(logger::stream[this -> _stream]) << e << std::endl;

	logger::_private::last = e;
	logger::_private::store.push_back(e);

	if ( logger::_private::store.size() > logger::max_log_entries )
		logger::_private::store.erase(logger::_private::store.begin());

	return *this;
}

template<typename Ch, typename Traits, typename Sequence>
void logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::_reset() {
	this -> _buf.clear();
	this -> _tag.clear();
	this -> _detail.clear();
	this -> _padding.clear();
	this -> _quiet = false;
	this -> _unique = false;
}

template<typename Ch, typename Traits, typename Sequence>
void logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::_parse() {

	begin_parse:

	if ( auto begin = std::find_if(this -> _buf.begin(), this -> _buf.end(), [](Ch c) { return c == 0002; }); begin != this -> _buf.end()) {
		if ( auto end = std::find_if(begin, this -> _buf.end(), [](Ch c) { return c == 0003; }); end != this -> _buf.end()) {
			if ( auto sep = std::find_if(begin, end, [](Ch c) { return c == ':'; }); sep != _buf.end()) {

				std::string t(begin, sep);
				std::string v(sep, end);
				if ( !t.empty() && t.front() == 2 )
					t.erase(0, 1);
				if ( !v.empty() && v.front() == ':' )
					v.erase(0, 1);
				if ( !v.empty() && v.back() == 3 )
					v.pop_back();

				if ( t == "tag" )
					this -> _tag = common::trim_ws(v);
				else if ( t == "detail" )
					this -> _detail = common::trim_ws(v);
				else if ( t == "padding" ) {

					std::string s = common::trim_ws(v);
					int c = 0;
					int w = 0;
					bool error = false;

					#if __cplusplus >= 202002L
					std::erase_if(s, [](char ch) { return std::string("\r\n\t\v ").find(ch) != std::string::npos; });
					#else
					s.erase(std::remove_if(s.begin(), s.end(),
						[](char ch) { return std::string("\r\n\t\v ").find(ch) != std::string::npos; }), s.end());
					#endif

					if ( auto p_sep = std::find_if(s.begin(), s.end(), [](Ch c) { return c == ','; }); p_sep != s.end()) {

						std::string s_c(s.begin(), p_sep);
						std::string s_w(p_sep, s.end());

						while ( !s_c.empty() && !std::isdigit(s_c.front()))
							s_c.erase(0, 1);
						while ( !s_c.empty() && !std::isdigit(s_c.back()))
							s_c.pop_back();
						while ( !s_w.empty() && !std::isdigit(s_w.front()))
							s_w.erase(0, 1);
						while ( !s_w.empty() && !std::isdigit(s_w.back()))
							s_w.pop_back();

						if ( !s_w.empty() && !s_c.empty() && common::is_number(s_c) && common::is_number(s_w)) {

							try {
								c = std::stoi(s_c);
							}
							catch ( const std::invalid_argument& e ) { c = 0; error = true; }
							catch ( const std::out_of_range& e ) { c = 0; error = true; }

							if ( !error ) {

								try {
									w = std::stoi(s_w);
								}
								catch ( const std::invalid_argument& e ) { c = 0; error = true; }
								catch ( const std::out_of_range& e ) { c = 0; error = true; }
							}

							if ( !error ) {

								if ( c == 0 || w == 0 )
									this -> _padding = logger::padding(0, 0);
								else
									this -> _padding = logger::padding((size_t)c, (size_t)w);
							}
						}
					}
				} else if ( t == "quiet" ) {
					this -> _quiet = v == "true" ? true : false;
				} else if ( t == "unique" ) {
					this -> _unique = v == "true" ? true : false;
				}

				this -> _buf.erase(begin, end);
				goto begin_parse;
			}
		}
	}

	#if __cplusplus >= 202002L
	std::erase_if(this -> _buf, [](char ch) { return ch == 2 || ch == 3; });
	#else
	this -> _buf.erase(std::remove_if(this -> _buf.begin(), this -> _buf.end(),
		[](char ch) { return ch == 2 || ch == 3; }), this -> _buf.end());
	#endif
}

template<typename Ch, typename Traits, typename Sequence>
int logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::_sync() {

	if ( !this -> _buf.empty() && this -> _buf.back() == '\n' )
		this -> _buf.pop_back();

	#if __cplusplus >= 202002L
	std::erase_if(this -> _buf, [](char ch) { return std::string("\r\v").find(ch) != std::string::npos; });
	#else
	this -> _buf.erase(std::remove_if(this -> _buf.begin(), this -> _buf.end(),
		[](char ch) { return std::string("\r\v").find(ch) != std::string::npos; }), this -> _buf.end());
	#endif
	std::replace_if(this -> _buf.begin(), this -> _buf.end(),
		[](char ch) { return std::string("\n\t").find(ch) != std::string::npos; }, ' ');

	if ( this -> _buf.size() != 0 )
		this -> _parse();

	if ( this -> _buf.size() == 0 ) {
		this -> _reset();
		return -1;
	}

	this -> _flush();
	this -> _reset();
	return 0;
}

template<typename Ch, typename Traits, typename Sequence>
typename logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::int_type logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::_overflow(int_type ch) {

	if ( traits_type::eq_int_type(ch, traits_type::eof()))
		return traits_type::eof();

	this -> _buf.push_back(traits_type::to_char_type(ch));
	return ch;
}

template<typename Ch, typename Traits, typename Sequence>
logger::basic_LOG_LEVEL<Ch, Traits, Sequence>& logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::operator [](const std::string& t) {
	this -> _tag = common::trim_ws(t);
	return *this;
}

template<typename Ch, typename Traits, typename Sequence>
const std::string logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::name() const {
	return this -> operator std::string();
}

template<typename Ch, typename Traits, typename Sequence>
uint8_t logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::id() const {
	return this -> _id;
}

template<typename Ch, typename Traits, typename Sequence>
logger::STREAM logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::stream() const {
	return this -> _stream;
}

template<typename Ch, typename Traits, typename Sequence>
void logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::change_logging_level(const uint8_t new_level) {
	this -> _id = new_level;
}

template<typename Ch, typename Traits, typename Sequence>
logger::basic_LOG_LEVEL<Ch, Traits, Sequence>::operator std::string() const {
	return this -> _name;
}

template<typename C>
std::ostream& logger::operator <<(std::ostream& os, logger::basic_LOG_LEVEL<C>& l) {
	os << l.operator std::string();
	return os;
}
