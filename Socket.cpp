#include "Socket.h"

char* StringToCharArr(string str) {
    char *arr = new char[str.length()];
    for (int i = 0; i < str.length(); i++) {
        arr[i] = str.at(i);
    }
    return arr;
}

GoBangTransmissionPcakage::GoBangTransmissionPcakage() {
    Command = -1;
    locationx = -1;
    locationy = -1;
    playerid = -1;
}

string GoBangTransmissionPcakage::PackageToString() {
    string str = "";
    if (Command == 1) {
        str += Command; str += ".";
        str += locationx; str += ".";
        str += locationy; str += ".";
        str += playerid;
    }
    else {
        str = to_string(Command);
    }
    return str;
}

QString GoBangTransmissionPcakage::PackageToQString() {
    return QString::fromStdString(PackageToString());
}

void GoBangTransmissionPcakage::UnPackageToIntArr(QString qstr) {
    string str = qstr.toStdString();
    if (str.at(0) != '1') {
        char tmp[1]; tmp[0] = str.at(0);
        Command = atoi(tmp);
        locationx = 0; locationy = 0; playerid = -1;
    }
    else {
        string *strarr = new string[4];
        int tmp = 0;
        for (int i = 0; i < str.length(); i++) {
            if (str.at(i) != '.') {
                strarr[tmp] += str.at(i);
            }
            else {
                tmp += 1;
            }
        }
        Command = atoi(StringToCharArr(strarr[0]));
        locationx = atoi(StringToCharArr(strarr[1]));
        locationy = atoi(StringToCharArr(strarr[2]));
        playerid = atoi(StringToCharArr(strarr[3]));
    }
}

void GoBangTransmissionPcakage::UnPackageToIntArr(string str) {
    if (str.at(0) != 1) {
        char tmp[1]; tmp[0] = str.at(0);
        Command = atoi(tmp);
        locationx = 0; locationy = 0; playerid = -1;
    }
    else {
        string *strarr = new string[4];
        int tmp = 0;
        for (int i = 0; i < str.length(); i++) {
            if (str.at(i) != '.') {
                strarr[tmp] += str.at(i);
            }
            else {
                tmp += 1;
            }
        }
        Command = atoi(StringToCharArr(strarr[0]));
        locationx = atoi(StringToCharArr(strarr[1]));
        locationy = atoi(StringToCharArr(strarr[2]));
        playerid = atoi(StringToCharArr(strarr[3]));
    }
}
