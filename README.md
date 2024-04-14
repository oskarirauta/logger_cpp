[![License:MIT](https://img.shields.io/badge/License-MIT-blue?style=plastic)](LICENSE)
[![C++ CI build](../../actions/workflows/build.yml/badge.svg)](../../actions/workflows/build.yml)
### logger_cpp
C++ logging system library for application's internal use

### <sub>Note</sub>
[logcpp](https://github.com/oskarirauta/logcpp) was renamed to logger_cpp.
logcpp is archived and will no longer be updated. It exists because some other
projects still are using it. Future development happens now on logger_cpp repository.

logger_cpp is a system that uses streams for logging, similarly as
logger_cpp is a system that can be used to log similarly
```std::cout <<``` would be used but with log levels and some other options.

### <sub>Note</sub>
This does not log to syslog, it is more a log system for storing log messages
and outputting them to screen, depending on your system, when executed from
init - propably entries will log to syslog as well as output of program
might be directed there. Standard output is used for logging, unless log level
used is error - when output goes to cerr. This behaviour is default and can
be modified.

## <sub>Some features</sub>

 - possibility to select logging level or make it completely silent
 - possibility to output log to file
 - logging history
 - supresses duplicate messages
	- journal contains count of repetions
	- on screen displays _(duplicate message atleast once)_
 - tag log entries
 - add detail description for entry (no screen output, only on a journal entry)

### <sub>Log Entry</sub>
Example of making log entry with logger:
```logger::info << "log this line to info level" << std::endl;```

### <sub>Tag entries</sub>
You can either use

```logger::info << logger::tag("my_tag") << "tagged message" << std::endl;```

or, you can use a subscript operator[] instead (v2 feature)

```logger::info["my_tag"] << "tagged message" << std::endl;```

### <sub>Entry's detail description</sub>
Add detail description to log entry:

```logger::info << "message title" << logger::detail("message detail description") << std::endl;```

### <sub>Custom log levels</sub>
Adding custom log levels is supported, just create them like this:

```logger::LOG_LEVEL my_level("my", 10, logger::std_out);```

Where last argument is optional, and can be aswell be std_err;
First argument is name, and second is level id. Default provided ids are:
error = 0, warning = 1, info = 2, verbose = 3, vverbose = 4, debug = 255

You can always change ids, for example like this:

```logger::info.change_logging_level(10);```

### <sub>Trimming</sub>
log entries are trimmed, line feeds and tabs ( \n and \t ) are converted to spaces,
and \r \v will be erased from tags, details and messages.

### <sub>Default log levels</sub>
 - error
 - warning
 - notice
 - info
 - verbose
 - vverbose (display's as verbose)
 - debug

when you set your log level, it always enables level's with higher priorites, meaning
that if you set log level to info, also error, warning and notice are enabled.

## <sub>Depencies</sub>

Library depends on my [common_cpp](https://github.com/oskarirauta/common_cpp) library
which contains some generally used functions.

## <sub>Importing</sub>

 - import common_cpp as a submodule to common
 - import logger_cpp as a submodule to logger
 - include common_cpp's Makefile.inc and logger_cpp's Makefile.inc in your Makefile
 - link with COMMON_OBJS and LOGGER_OBJS

or check example Makefile, notice that default for LOGGER_DIR is logger instead of logger_cpp.
Don't forget to #include "logger.hpp" in your project's code where you plan to use logger.
Remember to clone with recursive submodules, using --recursive-submodules argument.

Note: with default build system, you need objs directory in your project's root, this is path
where object files are built.

## <sub>Example</sub>

Sample code is provided.
