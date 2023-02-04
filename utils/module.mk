PCAP_ANALYZER_BUILD_DIRS += $(OBJ_DIR)/utils

pcap_time_o = $(OBJ_DIR)/utils/pcap_time.o

PCAP_ANALYZER_TARGETS_OBJ += $(pcap_time_o)

$(pcap_time_o): utils/pcap_time.cpp \
                utils/pcap_time.h
	$(CXX) -c $(CXXFLAGS) -o $(pcap_time_o) utils/pcap_time.cpp

pcap_time_ut_o = $(OBJ_DIR)/utils/pcap_time_ut.o

PCAP_ANALYZER_TARGETS_OBJ += $(pcap_time_ut_o)

$(pcap_time_ut_o): utils/pcap_time_ut.cpp \
                   utils/ut.h             \
                   $(pcap_time_o)
	$(CXX) -c $(CXXFLAGS) -o $(pcap_time_ut_o) utils/pcap_time_ut.cpp

pcap_time_ut = $(BIN_DIR)/pcap_time_ut

PCAP_ANALYZER_TARGETS_EXE += $(pcap_time_ut)

$(pcap_time_ut): $(pcap_time_ut_o)
	$(CXX) -o $(pcap_time_ut) $(pcap_time_ut_o) $(pcap_time_o)
	@echo run pcap_time_ut...
	$(pcap_time_ut)

