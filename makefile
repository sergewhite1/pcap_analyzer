all: libpcap_analyzer.a pcap_analyzer pcap_analyzer_ut

pcap_analyzer.o: pcap_analyzer.c pcap_analyzer.h
	gcc -c -std=c99 -g pcap_analyzer.c -o pcap_analyzer.o

libpcap_analyzer.a: pcap_analyzer.o
	ar rcs libpcap_analyzer.a pcap_analyzer.o

main.o: main.c
	gcc -c -std=c99 -g main.c -o main.o

pcap_analyzer: main.o
	gcc main.o -static  -lpcap_analyzer -L. -o pcap_analyzer

pcap_analyzer_ut.o: pcap_analyzer_ut.c
	gcc -c --std=c99 -g pcap_analyzer_ut.c -o pcap_analyzer_ut.o

pcap_analyzer_ut: libpcap_analyzer.a pcap_analyzer_ut.o
	gcc pcap_analyzer_ut.o -static  -lpcap_analyzer -L. -o pcap_analyzer_ut
