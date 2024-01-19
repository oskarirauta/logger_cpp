#pragma once

#include <iostream>
#include <string>

namespace logger {

	struct tag {
			std::string name;
			tag(const std::string& s) : name(s) {}

			bool operator ==(const tag& other);
			bool operator ==(const tag& other) const;
			operator std::string() const;

			friend std::ostream& operator <<(std::ostream& os, const tag& t);
	};

	std::ostream& operator <<(std::ostream& os, const logger::tag& t);
}
