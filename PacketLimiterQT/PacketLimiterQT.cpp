#include "PacketLimiterQT.h"
#include "ProcessedPackets.h"
#include "RateLimiter.h"
#include "PcapHandlers.h"


PacketLimiterQT::PacketLimiterQT(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setFixedSize(width(), height());
    connect(ui.Ok_Button, &QPushButton::clicked, this, &PacketLimiterQT::onButtonClicked);
}

void PacketLimiterQT::onButtonClicked() {
    fileLocation = ui.fileLocatiobField->displayText();
    std::string tempStr = fileLocation.toStdString();
    const char* fileLocationChar = tempStr.c_str();

   
    //C:/Users/Adilet/Documents/NPcap/Missed.pcap

    try {
        RateLimiter rateLimiter(rateMbps);
        PcapHandlers pcapHandle(fileLocationChar, "ProcessedPackets.pcap", rateLimiter);
        pcapHandle.openDumpers();
        pcapHandle.processPackets();

        packets = new ProcessedPackets(pcapHandle.getStoredPacketInfo());
        packets->show();
    }
    catch (const std::invalid_argument& e) {
        // Print error meessage if there is incorrect value for rate or packet size
        std::cerr << e.what() << std::endl;
    }
    catch (const std::runtime_error& e) {
        //Print error message if pcap file was not found or could not have been opened
        std::cerr << e.what() << std::endl;
    }
    hide();
}

PacketLimiterQT::~PacketLimiterQT()
{}
