pcap_time_o = $(OBJ_DIR)/utils/pcap_time.o

$(pcap_time_o): utils/pcap_time.cpp \
                utils/pcap_time.h
	$(CXX) -c $(CXXFLAGS) -o $(pcap_time_o) utils/pcap_time.cpp
