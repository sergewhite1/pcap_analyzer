BUILD_DIRS += $(OBJ_DIR)/pcap_analyzer_lib     \
              $(OBJ_DIR)/pcap_analyzer_lib/src \
              $(OBJ_DIR)/pcap_analyzer_lib/ut

pcap_analyzer_lib = $(LIB_DIR)/libpcap_analyzer.a


pcap_analyzer_o = $(OBJ_DIR)/pcap_analyzer_lib/src/pcap_analyzer.o

$(pcap_analyzer_o): pcap_analyzer_lib/src/pcap_analyzer.c \
                    pcap_analyzer_lib/include/pcap_analyzer.h
	$(CC) -c $(CFLAGS) -fPIC -o $(pcap_analyzer_o) pcap_analyzer_lib/src/pcap_analyzer.c

$(pcap_analyzer_lib): $(pcap_analyzer_o)
	ar rcs $(pcap_analyzer_lib) $(pcap_analyzer_o)

pcap_analyzer_ut_o = $(OBJ_DIR)/pcap_analyzer_lib/ut/pcap_analyzer_ut.o

$(pcap_analyzer_ut_o): pcap_analyzer_lib/ut/pcap_analyzer_ut.c
	$(CC) -c $(CFLAGS) pcap_analyzer_lib/ut/pcap_analyzer_ut.c -o $(pcap_analyzer_ut_o)

pcap_analyzer_ut = $(BIN_DIR)/pcap_analyzer_ut

$(pcap_analyzer_ut): $(pcap_analyzer_lib) $(pcap_analyzer_ut_o)
	$(CC) -o $(pcap_analyzer_ut) $(pcap_analyzer_ut_o) -L$(LIB_DIR) -lpcap_analyzer
	@echo run pcap_analyzer_ut...
	$(pcap_analyzer_ut)

PCAP_ANALYZER_TARGETS += $(pcap_analyzer_lib) $(pcap_analyzer_ut)
