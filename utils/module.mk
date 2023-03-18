PCAP_ANALYZER_BUILD_DIRS += $(OBJ_DIR)/utils

pcap_time_o = $(OBJ_DIR)/utils/pcap_time.o

PCAP_ANALYZER_TARGETS_OBJ += $(pcap_time_o)

$(pcap_time_o): utils/pcap_time.cpp \
                utils/pcap_time.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

pcap_time_ut_o = $(OBJ_DIR)/utils/pcap_time_ut.o

PCAP_ANALYZER_TARGETS_OBJ += $(pcap_time_ut_o)

$(pcap_time_ut_o): utils/pcap_time_ut.cpp \
                   utils/ut.h             \
                   $(pcap_time_o)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

pcap_time_ut = $(BIN_DIR)/pcap_time_ut

PCAP_ANALYZER_TARGETS_EXE += $(pcap_time_ut)

$(pcap_time_ut): $(pcap_time_ut_o) $(pcap_time_o)
	$(CXX) -o $@ $^
	@echo run pcap_time_ut...
	$(pcap_time_ut)

peak_rate_calc_o = $(OBJ_DIR)/utils/peak_rate_calc.o

PCAP_ANALYZER_TARGETS_OBJ += $(peak_rate_calc_o)

$(peak_rate_calc_o): utils/peak_rate_calc.cpp \
                     utils/peak_rate_calc.h   \
					 utils/pcap_time.h        \
					 pcap_analyzer_lib/include/pcap_analyzer.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

peak_rate_calc_ut_o = $(OBJ_DIR)/utils/peak_rate_calc_ut.o

PCAP_ANALYZER_TARGETS_OBJ += $(peak_rate_calc_ut_o)

$(peak_rate_calc_ut_o): utils/peak_rate_calc_ut.cpp \
                        utils/peak_rate_calc.h      \
						utils/ut.h                  \
						utils/pcap_time.h           \
						pcap_analyzer_lib/include/pcap_analyzer.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

peak_rate_calc_ut = $(BIN_DIR)/peak_rate_calc_ut

PCAP_ANALYZER_TARGETS_EXE += $(peak_rate_calc_ut)

$(peak_rate_calc_ut): $(peak_rate_calc_ut_o) $(peak_rate_calc_o) $(pcap_time_o)
	$(CXX) -o $@ $^
	@echo run peak_rate_calc_ut...
	$(peak_rate_calc_ut)
