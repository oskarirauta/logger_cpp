LOGGER_DIR?=logger
INCLUDES += -I./$(LOGGER_DIR)/include

LOGGER_OBJS:= \
	objs/logger_tag.o \
	objs/logger_detail.o \
	objs/logger_padding.o \
	objs/logger_entry.o \
	objs/logger.o

objs/logger_tag.o: $(LOGGER_DIR)/src/tag.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/logger_detail.o: $(LOGGER_DIR)/src/detail.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/logger_padding.o: $(LOGGER_DIR)/src/padding.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/logger_entry.o: $(LOGGER_DIR)/src/entry.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;

objs/logger.o: $(LOGGER_DIR)/src/logger.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<;
