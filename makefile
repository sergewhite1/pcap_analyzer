CC = gcc
CFLAGS = -std=c99 -Wall -g

all: libpcap_analyzer.a pcap_analyzer pcap_analyzer_ut

pcap_analyzer.o: pcap_analyzer.c pcap_analyzer.h
	$(CC) -c $(CFLAGS) pcap_analyzer.c -o pcap_analyzer.o

libpcap_analyzer.a: pcap_analyzer.o
	ar rcs libpcap_analyzer.a pcap_analyzer.o

main.o: main.c
	$(CC) -c $(CFLAGS)  main.c -o main.o

pcap_analyzer: main.o
	$(CC) main.o -lpcap_analyzer -L. -o pcap_analyzer

pcap_analyzer_ut.o: pcap_analyzer_ut.c
	$(CC) -c $(CFLAGS) pcap_analyzer_ut.c -o pcap_analyzer_ut.o

pcap_analyzer_ut: libpcap_analyzer.a pcap_analyzer_ut.o
	$(CC) pcap_analyzer_ut.o  -lpcap_analyzer -L. -o pcap_analyzer_ut

clean:
	rm libpcap_analyzer.a pcap_analyzer pcap_analyzer_ut *.o
