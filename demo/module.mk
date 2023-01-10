BUILD_DIRS += $(OBJ_DIR)/demo

demo = $(BIN_DIR)/demo

demo_o = $(OBJ_DIR)/demo/demo.o
 
$(demo_o): demo/demo.cpp     \
           utils/pcap_time.h
	$(CXX) -c $(CXXFLAGS) -o $(demo_o) demo/demo.cpp

$(demo): $(demo_o)               \
      out/lib/libpcap_analyzer.a \
      utils/pcap_time.h          \
      $(OBJ_DIR)/utils/pcap_time.o
	$(CXX) -o $(demo) $(demo_o) ${OBJ_DIR}/utils/pcap_time.o  -L$(LIB_DIR) -lpcap_analyzer

PCAP_ANALYZER_TARGETS += $(demo)
