pcap_time.o: utils/pcap_time.cpp \
             utils/pcap_time.h

	$(CXX) -c $(CXXFLAGS) -o $(OBJ_DIR)/utils/pcap_time.o utils/pcap_time.cpp
