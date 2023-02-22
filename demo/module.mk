PCAP_ANALYZER_BUILD_DIRS += $(OBJ_DIR)/demo

demo = $(BIN_DIR)/demo

PCAP_ANALYZER_TARGETS_EXE += $(demo)

demo_o = $(OBJ_DIR)/demo/demo.o

PCAP_ANALYZER_TARGETS_OBJ += $(demo_o)
 
$(demo_o): demo/demo.cpp     \
           utils/pcap_time.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(demo): $(demo_o)               \
      out/lib/libpcap_analyzer.a \
      utils/pcap_time.h          \
      $(OBJ_DIR)/utils/pcap_time.o
	$(CXX) -o $@ $(demo_o) ${OBJ_DIR}/utils/pcap_time.o  -L$(LIB_DIR) -lpcap_analyzer

