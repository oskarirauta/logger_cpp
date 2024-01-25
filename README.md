# logger_cpp
C++ application internal logging system

[![C++ CI build](https://github.com/oskarirauta/logger_cpp/actions/workflows/build.yml/badge.svg)](https://github.com/oskarirauta/logger_cpp/actions/workflows/build.yml)

Note: [logcpp](https://github.com/oskarirauta/logcpp) was renamed to logger_cpp.
logcpp is archived and will no longer be updated. It exists because some other
projects still are using it. Future development happens on logger_cpp repository.

logger_cpp is a system that can be used to log similarly
as ```std::cout <<``` would be used but with log levels.
Designed for background service applications mainly to provide a bit
more extensive logging facility than just using std::cout

 - possibility to select logging level or make it completely silent
 - possibility to output log to file
 - logging history
 - supresses duplicate messages
	- journal contains count of repetions
	- on screen displays _(duplicate message atleast once)_
 - tag log entries
 - add detail description for entry (no screen output, only on a journal entry)

### Log Entry

```logger::info << "log this line to info level" << std::endl;```

### Tag entries

You can either use

```logger::info << logger::tag("my_tag") << "tagged message" << std::endl;```

or, you can use a subscript operator[] instead (v2 feature)

```logger::info["my_tag"] << "tagged message" << std::endl;```

### Entry's detail description

Add detail description to log entry:

```logger::info << "message title" << logger::detail("message detail description") << std::endl;```

### Custom log levels

Adding custom log levels is supported, just create them like this:

```logger::LOG_LEVEL my_level("my", 10, logger::std_out);```

Where last argument is optional, and can be aswell be std_err;
First argument is name, and second is level id. Default provided ids are:
error = 0, warning = 1, info = 2, verbose = 3, vverbose = 4, debug = 255

You can always change ids, for example like this:

```logger::info.change_logging_level(10);```

### Trimming

log entries are trimmed, line feeds and tabs ( \n and \t ) are converted to spaces,
and \r \v will be erased from tags, details and messages.

### Default log levels

 - error
 - warning
 - notice
 - info
 - verbose
 - vverbose (display's as verbose)
 - debug

when you set your log level, it always enables level's with higher priorites, meaning
that if you set log level to info, also error, warning and notice are enabled.

### Depencies

Library depends on my [common_cpp](https://github.com/oskarirauta/common_cpp) library
which contains some generally used functions.

### Importing
 - import common_cpp as a submodule to common
 - import logger_cpp as a submodule to logger
 - include common_cpp's Makefile.inc and logger_cpp's Makefile.inc in your Makefile
 - link with COMMON_OBJS and LOGGER_OBJS

or check example Makefile, notice that default for LOGGER_DIR is logger instead of logger_cpp.
Don't forget to #include "logger.hpp" in your project's code where you plan to use logger.
Remember to clone with recursive submodules, using --recursive-submodules argument.

Note: with default build system, you need objs directory in your project's root, this is path
where object files are built.

### Example

Sample code is provided.

### Other

MIT-license
