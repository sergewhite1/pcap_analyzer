PCAP_ANALYZER_BUILD_DIRS += $(OBJ_DIR)/utils/data_rate

strict_peak_rate_calc_o = $(OBJ_DIR)/utils/data_rate/strict_peak_rate_calc.o

PCAP_ANALYZER_TARGETS_OBJ += $(strict_peak_rate_calc_o)

$(strict_peak_rate_calc_o): utils/data_rate/strict_peak_rate_calc.cpp \
                            utils/data_rate/strict_peak_rate_calc.h   \
                            utils/data_rate/peak_rate_calc.h          \
					        utils/pcap_time.h                         \
					        pcap_analyzer_lib/include/pcap_analyzer.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

strict_peak_rate_calc_ut_o = $(OBJ_DIR)/utils/data_rate/strict_peak_rate_calc_ut.o

PCAP_ANALYZER_TARGETS_OBJ += $(strict_peak_rate_calc_ut_o)

$(strict_peak_rate_calc_ut_o): utils/data_rate/strict_peak_rate_calc_ut.cpp \
                               utils/data_rate/strict_peak_rate_calc.h      \
                               utils/data_rate/peak_rate_calc.h             \
						       utils/ut.h                                   \
						       utils/pcap_time.h                            \
						       pcap_analyzer_lib/include/pcap_analyzer.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

strict_peak_rate_calc_ut = $(BIN_DIR)/strict_peak_rate_calc_ut

PCAP_ANALYZER_TARGETS_EXE += $(strict_peak_rate_calc_ut)

$(strict_peak_rate_calc_ut):                            \
  $(OBJ_DIR)/utils/data_rate/strict_peak_rate_calc_ut.o \
  $(OBJ_DIR)/utils/data_rate/strict_peak_rate_calc.o    \
  $(OBJ_DIR)/utils/pcap_time.o
	$(CXX) -o $@ $^
	@echo run strict_peak_rate_calc_ut...
	$@

sw_peak_rate_calc_o = $(OBJ_DIR)/utils/data_rate/sw_peak_rate_calc.o

PCAP_ANALYZER_TARGETS_OBJ += $(sw_peak_rate_calc_o)

$(sw_peak_rate_calc_o): utils/data_rate/sw_peak_rate_calc.cpp \
                        utils/data_rate/sw_peak_rate_calc.h   \
						utils/data_rate/peak_rate_calc.h      \
						utils/pcap_time.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

sw_peak_rate_calc_ut_o = $(OBJ_DIR)/utils/data_rate/sw_peak_rate_calc_ut.o

PCAP_ANALYZER_TARGETS_OBJ += $(sw_peak_rate_calc_ut_o)

$(sw_peak_rate_calc_ut_o): utils/data_rate/sw_peak_rate_calc_ut.cpp \
                           utils/data_rate/sw_peak_rate_calc.h      \
						   utils/data_rate/peak_rate_calc.h         \
						   utils/pcap_time.h                        \
						   utils/ut.h                               \
						   pcap_analyzer_lib/include/pcap_analyzer.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

sw_peak_rate_calc_ut = $(BIN_DIR)/sw_peak_rate_calc_ut

PCAP_ANALYZER_TARGETS_EXE += $(sw_peak_rate_calc_ut)

$(sw_peak_rate_calc_ut): $(OBJ_DIR)/utils/data_rate/sw_peak_rate_calc_ut.o \
                         $(OBJ_DIR)/utils/data_rate/sw_peak_rate_calc.o    \
						 $(OBJ_DIR)/utils/pcap_time.o
	$(CXX) -o $@ $^
	@echo run sw_peak_rate_calc_ut
	$@

rate_calc_o = $(OBJ_DIR)/utils/data_rate/rate_calc.o

PCAP_ANALYZER_TARGETS_OBJ += $(rate_calc_o)

$(rate_calc_o): utils/data_rate/rate_calc.cpp \
                utils/data_rate/rate_calc.h   \
                utils/pcap_time.h             \
                pcap_analyzer_lib/include/pcap_analyzer.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

rate_calc_ut_o = $(OBJ_DIR)/utils/data_rate/rate_calc_ut.o

PCAP_ANALYZER_TARGETS_OBJ += $(rate_calc_ut_o)

$(rate_calc_ut_o): utils/data_rate/rate_calc_ut.cpp \
                   utils/data_rate/rate_calc.h      \
                   utils/ut.h                       \
                   utils/pcap_time.h                \
                   pcap_analyzer_lib/include/pcap_analyzer.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

rate_calc_ut = $(BIN_DIR)/rate_calc_ut

PCAP_ANALYZER_TARGETS_EXE += $(rate_calc_ut)

$(rate_calc_ut): $(OBJ_DIR)/utils/data_rate/rate_calc_ut.o \
                 $(OBJ_DIR)/utils/data_rate/rate_calc.o    \
                 $(OBJ_DIR)/utils/pcap_time.o
	$(CXX) -o $@ $^
	@echo run $@
	$@

avg_rate_calc_o = $(OBJ_DIR)/utils/data_rate/avg_rate_calc.o

PCAP_ANALYZER_TARGETS_OBJ += $(avg_rate_calc_o)

$(avg_rate_calc_o): utils/data_rate/avg_rate_calc.cpp \
                    utils/data_rate/rate_calc.h       \
                    utils/pcap_time.h                 \
                    pcap_analyzer_lib/include/pcap_analyzer.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

avg_rate_calc_ut_o = $(OBJ_DIR)/utils/data_rate/avg_rate_calc_ut.o

PCAP_ANALYZER_TARGETS_OBJ += $(avg_rate_calc_ut_o)

$(avg_rate_calc_ut_o): utils/data_rate/avg_rate_calc_ut.cpp \
                       utils/ut.h                           \
                       utils/pcap_time.h                    \
                       pcap_analyzer_lib/include/pcap_analyzer.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

avg_rate_calc_ut = $(BIN_DIR)/avg_rate_calc_ut

PCAP_ANALYZER_TARGETS_EXE += $(avg_rate_calc_ut)

$(avg_rate_calc_ut): $(OBJ_DIR)/utils/data_rate/avg_rate_calc_ut.o \
                     $(OBJ_DIR)/utils/data_rate/rate_calc.o        \
					 $(OBJ_DIR)/utils/data_rate/avg_rate_calc.o    \
                     $(OBJ_DIR)/utils/pcap_time.o                  \
					 $(OBJ_DIR)/utils/data_rate/strict_peak_rate_calc.o
	$(CXX) -o $@ $^
	@echo run $@
	$@
