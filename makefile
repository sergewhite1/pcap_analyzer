CC        = gcc
CFLAGS    = -std=c99   -Wall -Wextra -Werror -g -I.

CXX       = g++
CXXFLAGS  = -std=c++11 -Wall -Wextra -Werror -g -I.

OUT_DIR   = out
BIN_DIR   = $(OUT_DIR)/bin
LIB_DIR   = $(OUT_DIR)/lib
OBJ_DIR   = $(OUT_DIR)/obj

# Build Directories (append in module.mk files)

BUILD_DIRS = $(OUT_DIR) $(BIN_DIR) $(LIB_DIR) $(OBJ_DIR)
PCAP_ANALYZER_TARGETS = make_build_dirs

all:

make_build_dirs:
	@for dir in $(BUILD_DIRS); do \
	  mkdir -p $$dir; \
	done

modules = $(shell find . -name module.mk)
include $(modules)


all: $(PCAP_ANALYZER_TARGETS)

clean:
	rm -rf out

