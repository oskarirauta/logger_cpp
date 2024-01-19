#pragma once

#include <iostream>
#include <string>

namespace logger {

	struct detail {
			std::string txt;
			detail(const std::string& s) : txt(s) {}

			bool operator ==(const detail& other);
			bool operator ==(const detail& other) const;
			operator std::string() const;

			friend std::ostream& operator <<(std::ostream& os, const detail& t);
	};

	std::ostream& operator <<(std::ostream& os, const logger::detail& d);
}
