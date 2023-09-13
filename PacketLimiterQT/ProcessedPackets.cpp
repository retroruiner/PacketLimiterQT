#include "ProcessedPackets.h"

ProcessedPackets::ProcessedPackets(std::vector<PcapHandlers::PacketInfo> packetInfo, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setFixedSize(width(), height());
	connect(ui.Close_Button, &QPushButton::clicked, this, &ProcessedPackets::onButtonClicked);

	ui.PacketsTable->setRowCount(packetInfo.size());
	
	for (size_t i = 0; i < packetInfo.size(); i++)
	{
		ui.PacketsTable->setItem(i, 0, new QTableWidgetItem(QString(QString::fromStdString(packetInfo.at(i).sourceIP))));
		ui.PacketsTable->setItem(i, 1, new QTableWidgetItem(QString(QString::fromStdString(packetInfo.at(i).destIP))));
		ui.PacketsTable->setItem(i, 2, new QTableWidgetItem(QString(QString::number(packetInfo.at(i).packetLength))));
		ui.PacketsTable->setItem(i, 3, new QTableWidgetItem(QString(QString::fromStdString(packetInfo.at(i).timestamp))));
	}
	
}

void ProcessedPackets::onButtonClicked() {
	this->close();
}

ProcessedPackets::~ProcessedPackets()
{}
