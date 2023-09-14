#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <pcap.h>
#include <string>
#include "RateLimiter.h"
#include <vector>



class PcapHandlers
{
public:
	typedef struct PacketInfo {
		std::string sourceIP;
		std::string destIP;
		int packetLength;
		std::string timestamp;
	}PacketInfo;


	PcapHandlers(const char* inputPcap, const char* outputPcap, RateLimiter& packetLimiter);
	bool openDumpers();
	void processPackets();
	int getTotalPack();
	int getMissedPack();
	int getProcessedPack();
	std::vector<PacketInfo> getStoredPacketInfo();
	~PcapHandlers();

private:
	RateLimiter& packetLimiter_;
	pcap_t* pcapHandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	const char* inputPcap_;
	const char* outputPcap_;
	pcap_dumper_t* pcapDumper;
	pcap_dumper_t* missedPcapDumper;
	pcap_t* missedPcapHandle;
	struct pcap_pkthdr header; //Structure provided by the pcap library that contains information about the captured packet, such as timestamp, length, and so on
	const u_char* packet_data;
	int packetsProcessed = 0;
	int packetsMissed = 0;
	int totalPackets = 0;

	/* 4 bytes IP address */
	typedef struct ip_address {
		u_char byte1;
		u_char byte2;
		u_char byte3;
		u_char byte4;
	}ip_address;

	/* IPv4 header */
	typedef struct ip_header {
		u_char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
		u_char  tos;            // Type of service 
		u_short tlen;           // Total length 
		u_short identification; // Identification
		u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
		u_char  ttl;            // Time to live
		u_char  proto;          // Protocol
		u_short crc;            // Header checksum
		ip_address  saddr;      // Source address
		ip_address  daddr;      // Destination address
		u_int   op_pad;         // Option + Padding
	}ip_header;

	std::vector<ip_address> storedIPAddresses;
	std::vector<int> storedPacketLengths;
	std::vector<time_t> storedTimestamps;

};

