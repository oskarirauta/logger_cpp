#pragma once

#include <cstdint>
#include <chrono>
#include <string>

namespace logger {

	struct entry {

		std::string name;
		uint8_t id;

		std::chrono::seconds timestamp = std::chrono::duration_cast<std::chrono::seconds>
			(std::chrono::system_clock::now().time_since_epoch());
		std::chrono::seconds timestamp_last = timestamp;

		std::string msg;
		std::string tag;
		std::string detail;
		int count = 0;

		bool operator ==(const entry& other);
		bool operator ==(const entry& other) const;
		operator std::string() const;

		bool has_detail() const;
		bool has_tag() const;
		const std::string detail_title() const;
		const std::string detail_spacing() const;
		const std::string get_timestamp() const;

		friend std::ostream& operator <<(std::ostream& os, const entry& t);
	};

	std::ostream& operator <<(std::ostream& os, const entry& e);
}
