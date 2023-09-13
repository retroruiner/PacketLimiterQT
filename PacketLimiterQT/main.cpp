#include "PacketLimiterQT.h"
#include "pcap.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PacketLimiterQT w;
    w.show();

    

    return a.exec();
}
