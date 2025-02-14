#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H

#include <iostream>
#include <stack>

using namespace std;

class ChessNode {
public:
    int* LOCATION();
    int PLAYERID();
    int STEPORDINAL();

    void SetLocation(int *location);
    void SetLocation(int x, int y);
    void SetPlayer(int playerid);
    void SetStep(int stepordinal);
    void SetChess(int *location, int playerid, int stepordinal);
    void SetChess(int x, int y, int playerid, int stepordinal);

    ChessNode();

private:
    int *Location;
    int PlayerID;
    int StepOrdinal;
};

class ChessRoute {
public:
    bool isEmpty();
    bool isChecked();
    int StepCount();
    int showFirstPlayerID();
    int showNextPlayerID();
    ChessNode RecentStep();

    void AddStep(ChessNode Step);
    void SetPlayers(int firstplayerid, int nextplayerid);
    void DelRecentStep();
    void DelTwoRecentStep();

    ChessRoute();

private:
    stack<ChessNode> Route;
    int FirstPlayer;
    int NextPlayer;
};

class CheckerBoard {
public:
    bool PlaceNode(int *location, int placeplayerid);
    bool PlaceNode(int location_x, int location_y, int placeplayerid);
    void SetSize(int size);

    ChessNode** Board;
    ChessRoute Route;

    CheckerBoard(int size);
    CheckerBoard();

private:
    int Size;

    void ReSetCheckerBoard();
};

#endif // CHECKERBOARD_H
