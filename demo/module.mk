demo = out/bin/demo
 
demo.o: demo/demo.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJ_DIR)/demo/demo.o demo/demo.cpp

demo: demo.o                     \
      out/lib/libpcap_analyzer.a \
      utils/pcap_time.h          \
      pcap_time.o
	$(CXX) -o $(BIN_DIR)/demo ${OBJ_DIR}/demo/demo.o ${OBJ_DIR}/utils/pcap_time.o  -L$(LIB_DIR) -lpcap_analyzer
