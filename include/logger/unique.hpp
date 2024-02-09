#pragma once

#include <iostream>
#include <string>

namespace logger {

	struct unique {
			bool state;
			unique(bool b = true) : state(b) {}

			bool operator ==(const unique& other);
			bool operator ==(const unique& other) const;
			operator std::string() const;

			friend std::ostream& operator <<(std::ostream& os, const unique& t);
	};

	std::ostream& operator <<(std::ostream& os, const logger::unique& d);
}
