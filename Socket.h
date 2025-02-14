#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <QString>
#include <QDebug>

using namespace std;

char* StringToCharArr(string str);

class GoBangTransmissionPcakage {
public:
    int Command;
    int locationx;
    int locationy;
    int playerid;

    QString PackageToQString();
    string PackageToString();
    void UnPackageToIntArr(QString qstr);
    void UnPackageToIntArr(string str);

    GoBangTransmissionPcakage();
};

#endif // SOCKET_H
