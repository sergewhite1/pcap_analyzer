CC        = gcc
CFLAGS    = -std=c99   -Wall -Wextra -Werror -g -I.

CXX       = g++
CXXFLAGS  = -std=c++11 -Wall -Wextra -Werror -g -I.

OUT_DIR   = out
BIN_DIR   = $(OUT_DIR)/bin
LIB_DIR   = $(OUT_DIR)/lib
OBJ_DIR   = $(OUT_DIR)/obj

# Special variable (append in module.mk files)

PCAP_ANALYZER_BUILD_DIRS  = $(OUT_DIR) $(BIN_DIR) $(LIB_DIR) $(OBJ_DIR)
PCAP_ANALYZER_TARGETS_OBJ =
PCAP_ANALYZER_TARGETS_LIB =
PCAP_ANALYZER_TARGETS_EXE =


all:

make_build_dirs:
	@for dir in $(PCAP_ANALYZER_BUILD_DIRS); do \
	  mkdir -p $$dir; \
	done

modules = $(shell find . -name module.mk)
include $(modules)


all: make_build_dirs              \
     $(PCAP_ANALYZER_TARGETS_OBJ) \
     $(PCAP_ANALYZER_TARGETS_LIB) \
     $(PCAP_ANALYZER_TARGETS_EXE)

.PHONY: clean
clean:
	rm -rf out

