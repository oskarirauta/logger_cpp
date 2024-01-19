#pragma once

#include <iostream>
#include <string>

namespace logger {

	struct padding {
			size_t count;
			size_t width;

			padding() : count(0), width(0) {}
			padding(size_t n, size_t w = 1) : count(n), width(w) {}

			bool operator ==(const padding& other);
			bool operator ==(const padding& other) const;
			operator std::string() const;

			bool empty();
			bool empty() const;
			void clear();

			friend std::ostream& operator <<(std::ostream& os, const padding& p);
	};

	std::ostream& operator <<(std::ostream& os, const logger::padding& p);
}
