CC        = gcc
CFLAGS    = -std=c99   -Wall -Wextra -Werror -g

CXX       = g++
CXXFLAGS  = -std=c++11 -Wall -Wextra -Werror -g

OUT_DIR   = out
BIN_DIR   = $(OUT_DIR)/bin
LIB_DIR   = $(OUT_DIR)/lib
OBJ_DIR   = $(OUT_DIR)/obj

all:                     \
     directories         \
     libpcap_analyzer.a  \
     pcap_analyzer_ut    \
     demo                \

directories:
	mkdir -p $(OUT_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/demo
	mkdir -p ${OBJ_DIR}/pcap_analyzer_lib
	mkdir -p ${OBJ_DIR}/pcap_analyzer_lib/src
	mkdir -p ${OBJ_DIR}/pcap_analyzer_lib/ut
	mkdir -p $(BIN_DIR)
	mkdir -p $(LIB_DIR)

libpcap_analyzer.a: pcap_analyzer.o
	ar rcs ${LIB_DIR}/libpcap_analyzer.a ${OBJ_DIR}/pcap_analyzer_lib/src/pcap_analyzer.o

pcap_analyzer.o: pcap_analyzer_lib/src/pcap_analyzer.c \
                 pcap_analyzer_lib/include/pcap_analyzer.h
	$(CC) -c $(CFLAGS) -fPIC -o ${OBJ_DIR}/pcap_analyzer_lib/src/pcap_analyzer.o pcap_analyzer_lib/src/pcap_analyzer.c

pcap_analyzer_ut: libpcap_analyzer.a pcap_analyzer_ut.o
	$(CC) -o $(BIN_DIR)/pcap_analyzer_ut $(OBJ_DIR)/pcap_analyzer_lib/ut/pcap_analyzer_ut.o -L$(LIB_DIR) -lpcap_analyzer
	$(BIN_DIR)/pcap_analyzer_ut

pcap_analyzer_ut.o: pcap_analyzer_lib/ut/pcap_analyzer_ut.c
	$(CC) -c $(CFLAGS) pcap_analyzer_lib/ut/pcap_analyzer_ut.c -o $(OBJ_DIR)/pcap_analyzer_lib/ut/pcap_analyzer_ut.o

demo: demo.o
	$(CXX) -o $(BIN_DIR)/demo ${OBJ_DIR}/demo/demo.o -L$(LIB_DIR) -lpcap_analyzer

demo.o: demo/demo.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJ_DIR)/demo/demo.o demo/demo.cpp

clean:
	rm -rf out
