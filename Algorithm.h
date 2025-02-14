#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>
#include <QFile>
#include "CheckerBoard.h"

using namespace std;

enum GameMode {LocalNon, Local, Entertain, Network, AI};

class GoBangAlgorithm {
public:
    string* GoBangScanner(ChessNode ScanNode, ChessNode** Board, ChessRoute r, int Size);
    bool bKMP(string StrLong, string StrShort);
    int iKMP(string StrLong, string StrShort);
    int GoBangJudger(ChessRoute r, ChessNode **Board, int Size, GameMode mode);

private:
    char* StringToCharArr(string str);
    string QStringToString(QString qstr);
    QString StringToQString(string str);
    string SubStringMix(string left, string right);
    string ChessNodeTranslate(ChessNode a, ChessRoute r);
    string StringReverse(string str);
};

#endif // ALGORITHM_H
