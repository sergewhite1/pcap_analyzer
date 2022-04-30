CC        = gcc
CFLAGS    = -std=c99   -Wall -Wextra -Werror -g

CXX       = g++
CXXFLAGS  = -std=c++11 -Wall -Wextra -Werror -g

all: libpcap_analyzer.a pcap_analyzer_ut analyzer

libpcap_analyzer.a: pcap_analyzer.o
	ar rcs libpcap_analyzer.a pcap_analyzer.o

pcap_analyzer.o: pcap_analyzer.c pcap_analyzer.h
	$(CC) -c $(CFLAGS) -fPIC -o pcap_analyzer.o pcap_analyzer.c

pcap_analyzer_ut: libpcap_analyzer.a pcap_analyzer_ut.o
	$(CC) -o pcap_analyzer_ut pcap_analyzer_ut.o -L. -lpcap_analyzer

pcap_analyzer_ut.o: pcap_analyzer_ut.c
	$(CC) -c $(CFLAGS) pcap_analyzer_ut.c -o pcap_analyzer_ut.o

analyzer: analyzer.o
	$(CXX) -o analyzer analyzer.o -L. -lpcap_analyzer

analyzer.o: analyzer.cpp
	$(CXX) -c $(CXXFLAGS) -o analyzer.o analyzer.cpp

clean:
	rm libpcap_analyzer.a pcap_analyzer_ut analyzer *.o
