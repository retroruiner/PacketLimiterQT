#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PacketLimiterQT.h"
#include "ProcessedPackets.h"

class PacketLimiterQT : public QMainWindow
{
    Q_OBJECT

public:
    PacketLimiterQT(QWidget *parent = nullptr);
    ~PacketLimiterQT();

private:
    Ui::PacketLimiterQTClass ui;
    ProcessedPackets *packets;
    QString fileLocation;
    int rateMbps;

    void onButtonClicked();
};
