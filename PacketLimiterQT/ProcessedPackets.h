#pragma once

#include <QWidget>
#include "ui_ProcessedPackets.h"
#include "PcapHandlers.h"

class ProcessedPackets : public QWidget
{
	Q_OBJECT

public:
	ProcessedPackets(std::vector<PcapHandlers::PacketInfo> packetInfo, QWidget *parent = nullptr);
	
	~ProcessedPackets();

private:
	Ui::ProcessedPacketsClass ui;
	void onButtonClicked();
};
