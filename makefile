CC = gcc
CFLAGS = -std=c99 -Wall -Werror -g

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Werror -g

all: libpcap_analyzer.so pcap_analyzer pcap_analyzer_ut

pcap_analyzer.o: pcap_analyzer.c pcap_analyzer.h
	$(CC) -c $(CFLAGS) -fpic  pcap_analyzer.c -o pcap_analyzer.o

libpcap_analyzer.so: pcap_analyzer.o
	gcc -shared -o libpcap_analyzer.so pcap_analyzer.o

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS)  main.cpp -o main.o

pcap_analyzer: main.o
	$(CXX) main.o -lpcap_analyzer -L. -o pcap_analyzer

pcap_analyzer_ut.o: pcap_analyzer_ut.c
	$(CC) -c $(CFLAGS) pcap_analyzer_ut.c -o pcap_analyzer_ut.o

pcap_analyzer_ut: libpcap_analyzer.so pcap_analyzer_ut.o
	$(CC) pcap_analyzer_ut.o  -lpcap_analyzer -L. -o pcap_analyzer_ut

clean:
	rm libpcap_analyzer.so pcap_analyzer pcap_analyzer_ut *.o
