#pragma once

#include <iostream>
#include <string>

namespace logger {

	struct quiet {
			bool state;
			quiet(bool b = true) : state(b) {}

			bool operator ==(const quiet& other);
			bool operator ==(const quiet& other) const;
			operator std::string() const;

			friend std::ostream& operator <<(std::ostream& os, const quiet& t);
	};

	std::ostream& operator <<(std::ostream& os, const logger::quiet& d);
}
