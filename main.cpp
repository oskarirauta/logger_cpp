#include <iostream>
#include <string>
#include "logger.hpp"

int main(int argc, char **argv) {

	//logger::loglevel(logger::verbose);
	//logger::silence = true;

	std::cout << "logger example" << std::endl;
	std::cout << "default logging level is info(2), currently set logging level is " << logger::log_level << std::endl;
	std::cout << "this is cout outputting to standard output: test - now beginning to output tests to logging streams\n" << std::endl;

	logger::info << std::string("log entry #1 to info level: item1") << std::endl;

	logger::error << "log entry #2 to error level:" << " item2" << " item3" << " item4";
	logger::debug << "log entry #3 to debug level: item5" << " item6";

	logger::error << std::endl;
	logger::debug << " item7" << std::endl;

	logger::info << "log entry #4 to info level: item8" << logger::detail("Extended description for item8") << std::endl;
	logger::info << "log entry #5 to info level: item9" << std::endl;
	logger::info << "log entry #6 to info level: item10" << std::endl;
	// duplication test
	logger::info << "info level: dup test" << std::endl;
	logger::info << "info level: dup test" << logger::detail("extension for dup test") << std::endl;
	logger::info << "info level: dup test" << std::endl;

	logger::info << logger::padding(10) << logger::padding(4) << "entry with padding" << std::endl;
	logger::info << logger::padding(10) << logger::padding() << "entry with padding cancelled" << std::endl;

	/*
	// force clear detailed description on update
	logger::info << "info level: dup test" << logger::detail("") << std::endl;
	*/

	logger::verbose << "log entry #7 to verbose level: item11" << std::endl;
	logger::info << "info level: dup test" << std::endl;

	// termination logger::endl (instead of '\n' or std::endl) adds entry
	// to journal without screen output
	logger::info << "log entry #8 to info level";
	logger::info << logger::detail("entry only without screen output") << std::endl;

	// only message1 is accepted
	std::cout << "adding \"message1\\nmessage2\\nmessage3 << std::endl to verbose level, only message1 is accepted (a caveat)" << std::endl;
	logger::verbose << "message1\nmessage2\nmessage3" << std::endl;
	logger::vverbose << "this message is sent to vverbose facility, but displays as verbose" << std::endl;

	// but this works..
	logger::verbose << "message4\n" << "message5" << "\n" << "message6" << std::endl;

	std::cout << "\nLog level for t is \"" << logger::verbose << "\" (should be verbose)" << std::endl;

	logger::info << logger::tag("example") << "log entry with tag" << std::endl;
	logger::info["tagged"] << "another entry with tag, but this time defined with subscript operator []" << std::endl;

	logger::info << logger::quiet() << "this entry is quiet, it will though be stored in a filestream and history, but not to stream" << std::endl;

	std::cout << "\nlog history(max 20 entries):" << std::endl;
	std::vector<logger::entry> hist = logger::history(20);
	for ( auto it = hist.begin(); it != hist.end(); ++it ) {
		std::cout << it -> name << ( it -> tag.empty() ? "" : ( " [" + it -> tag + "]" )) << ": " << it -> msg << ( it -> count > 1 ? ( " cnt: " + std::to_string(it -> count)) : "" ) << std::endl;
		if ( it -> has_detail())
			std::cout << it -> detail_spacing() << it -> detail << std::endl;
	}

	return 0;
}
