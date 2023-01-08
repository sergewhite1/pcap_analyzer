pcap_analyzer_lib = out/lib/libpcap_analyzer.a

pcap_analyzer.o: pcap_analyzer_lib/src/pcap_analyzer.c \
                 pcap_analyzer_lib/include/pcap_analyzer.h
	$(CC) -c $(CFLAGS) -fPIC -o ${OBJ_DIR}/pcap_analyzer_lib/src/pcap_analyzer.o pcap_analyzer_lib/src/pcap_analyzer.c

pcap_analyzer_lib: pcap_analyzer.o
	ar rcs ${LIB_DIR}/libpcap_analyzer.a ${OBJ_DIR}/pcap_analyzer_lib/src/pcap_analyzer.o

pcap_analyzer_ut.o: pcap_analyzer_lib/ut/pcap_analyzer_ut.c
	$(CC) -c $(CFLAGS) pcap_analyzer_lib/ut/pcap_analyzer_ut.c -o $(OBJ_DIR)/pcap_analyzer_lib/ut/pcap_analyzer_ut.o

pcap_analyzer_ut: pcap_analyzer_lib pcap_analyzer_ut.o
	$(CC) -o $(BIN_DIR)/pcap_analyzer_ut $(OBJ_DIR)/pcap_analyzer_lib/ut/pcap_analyzer_ut.o -L$(LIB_DIR) -lpcap_analyzer
	$(BIN_DIR)/pcap_analyzer_ut

