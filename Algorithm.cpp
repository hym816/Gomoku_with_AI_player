#include "Algorithm.h"

char * GoBangAlgorithm::StringToCharArr(string str) {
    char *arr = new char[str.length()];
    for (int i = 0; i < str.length(); i++) {
        arr[i] = str.at(i);
    }
    return arr;
}

string GoBangAlgorithm::QStringToString(QString qstr) {
    return qstr.toStdString();
}

QString GoBangAlgorithm::StringToQString(string str) {
    return QString::fromStdString(str);
}

bool GoBangAlgorithm::bKMP(string StrLong, string StrShort) {
    char* StrLongArr = StringToCharArr(StrLong);
    char* StrShortArr = StringToCharArr(StrShort);
    for (int i = 0; i < StrLong.length();) {
        int tmp = i;
        for (int j = 0; j < StrShort.length();) {
            if (StrLongArr[i] == StrShortArr[j]) {
                i++; j++;
                if (j == StrShort.length()) {
                    return true;
                }
            }
            else {
                i = tmp+1; j = 0;
                break;
            }
        }
    }
    return false;
}

int GoBangAlgorithm::iKMP(string StrLong, string StrShort) {
    int count = 0;
    char* StrLongArr = StringToCharArr(StrLong);
    char* StrShortArr = StringToCharArr(StrShort);
    for (int i = 0; i < StrLong.length();) {
        int tmp = i;
        for (int j = 0; j < StrShort.length();) {
            if (StrLongArr[i] == StrShortArr[j]) {
                i++; j++;
                if (j == StrShort.length()) {
                    count += 1;
                    i = tmp+1; j = 0;
                    break;
                }
            }
            else {
                i = tmp+1; j = 0;
                break;
            }
        }
    }
    return count;
}

string GoBangAlgorithm::StringReverse(string str) {
    string tmp = "";
    for (int i = str.length()-1; i >= 0; i--) {
        tmp += str.at(i);
    }
    return tmp;
}

string GoBangAlgorithm::SubStringMix(string left, string right) {
    return StringReverse(left) + right;
}

string GoBangAlgorithm::ChessNodeTranslate(ChessNode a, ChessRoute r) {
    if (a.PLAYERID() == r.showFirstPlayerID()) {
        return "f";
    }
    else if (a.PLAYERID() == r.showNextPlayerID()) {
        return "n";
    }
    else {
        return "?";
    }
}

string * GoBangAlgorithm::GoBangScanner(ChessNode ScanNode, ChessNode **Board, ChessRoute r, int Size) {
    string *arr = new string[8];
    for (int i = 0; i < 4; i++) {
        arr[i] = "";
    }
    // arr[0]: -
    for (int i = 1; i <= Size; i++) {
        arr[0] += ChessNodeTranslate(Board[ScanNode.LOCATION()[0]][i],r);
    }

    // arr[1]: |
    for (int i = 1; i <= Size; i++) {
        arr[1] += ChessNodeTranslate(Board[i][ScanNode.LOCATION()[1]],r);
    }

    // arr[2]: /
    for (int i = ScanNode.LOCATION()[0], j = ScanNode.LOCATION()[1]; i <= Size && j >= 1; i++, j--) {
        arr[2] += ChessNodeTranslate(Board[i][j],r);
    }
    string tmp2 = arr[2]; arr[2] = "";
    for (int i = ScanNode.LOCATION()[0]-1, j = ScanNode.LOCATION()[1]+1; i >= 1 && j <= Size; i--, j++) {
        arr[2] += ChessNodeTranslate(Board[i][j],r);
    }
    arr[2] = SubStringMix(tmp2, arr[2]);

    // arr[3]: "\"
    for (int i = ScanNode.LOCATION()[0], j = ScanNode.LOCATION()[1]; i >= 1 && j >= 1; i--, j--) {
        arr[3] += ChessNodeTranslate(Board[i][j],r);
    }
    string tmp3 = arr[3]; arr[3] = "";
    for (int i = ScanNode.LOCATION()[0]+1, j = ScanNode.LOCATION()[1]+1; i <= Size && j <= Size; i++, j++) {
        arr[3] += ChessNodeTranslate(Board[i][j],r);
    }
    arr[3] = SubStringMix(tmp3,arr[3]);

    for (int i = 4; i < 8; i++) {
        arr[i] = StringReverse(arr[i-4]);
    }

    return arr;
}

int GoBangAlgorithm::GoBangJudger(ChessRoute r, ChessNode **Board, int Size, GameMode mode) {
    bool isFirstPlayer = (r.RecentStep().PLAYERID() == r.showFirstPlayerID());
    string* ScanRoute = GoBangScanner(r.RecentStep(),Board,r,Size);
    if (isFirstPlayer) {
        // 判断长连禁手
        if (mode == Local) {
            bool tmpc[4] = {false}; bool tag = false;
            for (int i = 0; i < 4; i++) {
                if (bKMP(ScanRoute[i],"ffffff")) {
                    tmpc[i] = true;
                    tag = true;
                }
            }
            for (int i = 0; i < 4; i++) {
                if (tmpc[i] == false) {
                    if (bKMP(ScanRoute[i],"fffff")) {
                        return r.showFirstPlayerID();
                    }
                }
            }
            if (tag) {
                return r.showNextPlayerID();
            }
        }

        // 判断胜负
        for (int i = 0; i < 4; i++) {
            if (bKMP(ScanRoute[i],"fffff")) {
                return r.showFirstPlayerID();
            }
        }

        if (mode == Local) {
            string BannedRoute3[] = {"?fff?","?ff?f?"};
            double BannedRouteWeight3[] = {0.5,1};
            string BannedRoute4[] = {"fff?f","nffff?","ffff?n","?ffff?"};
            double BannedRouteWeight4[] = {1,1,1,0.5};

            // 判断三三禁手
            double Weight3 = 0;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 2; j++) {
                    Weight3 += BannedRouteWeight3[j]*iKMP(ScanRoute[i],BannedRoute3[j]);
                }
            }
            if (Weight3 > 1.1) {
                return r.showNextPlayerID();
            }

            // 判断四四禁手
            double Weight4 = 0;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 4; j++) {
                    Weight4 += BannedRouteWeight4[j]*iKMP(ScanRoute[i],BannedRoute4[j]);
                }
            }
            if (Weight4 > 1.1) {
                return r.showNextPlayerID();
            }
        }
    }
    else {
        for (int i = 0; i < 8; i++) {
            if (bKMP(ScanRoute[i],"nnnnn")) {
                return r.showNextPlayerID();
            }
        }
    }
    return -1;
}
