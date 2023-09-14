#include "PcapHandlers.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <pcap.h>

PcapHandlers::PcapHandlers(const char* inputPcap, const char* outputPcap, RateLimiter& packetLimiter) : inputPcap_(inputPcap), outputPcap_(outputPcap), packetLimiter_(packetLimiter) {
    pcapHandle = pcap_open_offline(inputPcap_, errbuf);
    if (pcapHandle == nullptr) {
        throw std::runtime_error("Error opening input pcap file");
    }
}

bool PcapHandlers::openDumpers() {
    // Create a pcap dumper for the processed packets
    pcapDumper = pcap_dump_open(pcapHandle, "processed.pcap");
    if (pcapDumper == nullptr) {
        throw std::runtime_error("Error creating output pcap file.");
        pcap_close(pcapHandle);
        return false;
    }

    // Create a pcap dumper for the missed packets
    missedPcapHandle = pcap_open_dead(DLT_EN10MB, 65535);
    missedPcapDumper = pcap_dump_open(missedPcapHandle, outputPcap_);

    return true;
}

void PcapHandlers::processPackets() {
    while ((packet_data = pcap_next(pcapHandle, &header)) != nullptr) {
        totalPackets++;
        // Try to consume the packet using the packetLimiter
        if (packetLimiter_.tryConsume(header.len)) {
            // Packet consumed within the rate, dump to processed pcap file
            pcap_dump((u_char*)pcapDumper, &header, packet_data);

            //retireve the position of the ip header
            ip_header* ih = (ip_header*)(packet_data + 18); //length of ethernet header
        
            ip_address source_ip = ih->saddr;
            ip_address dest_ip = ih->daddr;
            storedIPAddresses.push_back(source_ip);
            storedIPAddresses.push_back(dest_ip);

             // Extract packet length
            int packet_length = header.len;

            // Extract timestamp
            time_t timestamp = header.ts.tv_sec;

            // Add packet length and timestamp to the vectors
            storedPacketLengths.push_back(packet_length);
            storedTimestamps.push_back(timestamp);

            packetsProcessed++;
        }
        else {
            // Packet not consumed within the rate, dump to missed pcap file
            pcap_dump((u_char*)missedPcapDumper, &header, packet_data);
            packetsMissed++;
        }
    }
}

int PcapHandlers::getTotalPack() {
    return totalPackets;
}

int PcapHandlers::getMissedPack() {
    return packetsMissed;
}

int PcapHandlers::getProcessedPack() {
    return packetsProcessed;
}

std::vector<PcapHandlers::PacketInfo> PcapHandlers::getStoredPacketInfo()
{
    std::vector<PacketInfo> packetInfoList;
    for (size_t i = 0; i < storedIPAddresses.size(); i += 2) {
        const ip_address& source_ip = storedIPAddresses[i];
        const ip_address& dest_ip = storedIPAddresses[i + 1];

        // Retrieve packet length and timestamp
        int packet_length = storedPacketLengths[i / 2];
        time_t timestamp = storedTimestamps[i / 2];

        // Convert timestamp to a readable format
        struct tm ltime;
        char timestr[16];
        localtime_s(&ltime, &timestamp);
        strftime(timestr, sizeof timestr, "%H:%M:%S", &ltime);

        // Build IP address strings
        std::string source_ip_str = std::to_string(source_ip.byte1) + "." +
            std::to_string(source_ip.byte2) + "." +
            std::to_string(source_ip.byte3) + "." +
            std::to_string(source_ip.byte4);

        std::string dest_ip_str = std::to_string(dest_ip.byte1) + "." +
            std::to_string(dest_ip.byte2) + "." +
            std::to_string(dest_ip.byte3) + "." +
            std::to_string(dest_ip.byte4);

        // Create a PacketInfo structure and add it to the list
        PacketInfo packetInfo;
        packetInfo.sourceIP = source_ip_str;
        packetInfo.destIP = dest_ip_str;
        packetInfo.packetLength = packet_length;
        std::string time(timestr);
        packetInfo.timestamp = time;

        packetInfoList.push_back(packetInfo);
    }

    return packetInfoList;
}

//Close all the dumpers and handlers
PcapHandlers::~PcapHandlers() {
    pcap_dump_close(pcapDumper);
    pcap_dump_close(missedPcapDumper);
    pcap_close(pcapHandle);
    pcap_close(missedPcapHandle);
}