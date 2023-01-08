CC        = gcc
CFLAGS    = -std=c99   -Wall -Wextra -Werror -g -I.

CXX       = g++
CXXFLAGS  = -std=c++11 -Wall -Wextra -Werror -g -I.

OUT_DIR   = out
BIN_DIR   = $(OUT_DIR)/bin
LIB_DIR   = $(OUT_DIR)/lib
OBJ_DIR   = $(OUT_DIR)/obj

all:                     \
     directories         \
     pcap_analyzer_lib   \
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

include demo/module.mk
include pcap_analyzer_lib/module.mk

clean:
	rm -rf out
