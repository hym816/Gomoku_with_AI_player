#include "CheckerBoard.h"

ChessNode::ChessNode() {
    Location = new int[2];
    PlayerID = -1;
    StepOrdinal = -1;
}

void ChessNode::SetChess(int x, int y, int playerid, int stepordinal) {
    Location[0] = x; Location[1] = y;
    PlayerID = playerid; StepOrdinal = stepordinal;
}

void ChessNode::SetChess(int *location, int playerid, int stepordinal) {
    Location[0] = location[0]; Location[1] = location[1];
    PlayerID = playerid; StepOrdinal = stepordinal;
}

void ChessNode::SetLocation(int x, int y) {
    Location[0] = x; Location[1] = y;
}

void ChessNode::SetLocation(int *location) {
    Location[0] = location[0]; Location[1] = location[1];
}

void ChessNode::SetPlayer(int playerid) {
    PlayerID = playerid;
}

void ChessNode::SetStep(int stepordinal) {
    StepOrdinal = stepordinal;
}

int * ChessNode::LOCATION() {
    int *arr = new int[2];
    arr[0] = Location[0]; arr[1] = Location[1];
    return arr;
}

int ChessNode::PLAYERID() {
    return PlayerID;
}

int ChessNode::STEPORDINAL() {
    return StepOrdinal;
}

ChessRoute::ChessRoute() {
    FirstPlayer = -1;
    NextPlayer = -1;
}

void ChessRoute::SetPlayers(int firstplayerid, int nextplayerid) {
    FirstPlayer = firstplayerid; NextPlayer = nextplayerid;
}

void ChessRoute::AddStep(ChessNode Step) {
    Route.push(Step);
}

ChessNode ChessRoute::RecentStep() {
    return Route.top();
}

bool ChessRoute::isChecked() {
    if (isEmpty()) {
        return true;
    }
    else {
        if (StepCount() % 2 == 1)
        {
            if (RecentStep().PLAYERID() == FirstPlayer){
                return true;
            }
            return false;
        }
        else
        {
            if (RecentStep().PLAYERID() == NextPlayer) {
                return true;
            }
            return false;
        }

    }
}

bool ChessRoute::isEmpty() {
    return Route.empty();
}

int ChessRoute::StepCount() {
    return Route.size();
}

int ChessRoute::showFirstPlayerID() {
    return FirstPlayer;
}

int ChessRoute::showNextPlayerID() {
    return NextPlayer;
}

void ChessRoute::DelRecentStep() {
    Route.pop();
}

void ChessRoute::DelTwoRecentStep() {
    Route.pop();
    Route.pop();
}

CheckerBoard::CheckerBoard(int size) {
    Size = size;
    Board = new ChessNode*[Size+2];
    for (int i = 0; i < Size+2; i++) {
        Board[i] = new ChessNode[Size+2];
    }
    for (int i = 1; i <= Size; i++) {
        for (int j = 1; j <= Size; j++) {
            Board[i][j].SetChess(i,j,-1,-1);
        }
    }
    Route.AddStep(Board[0][0]);
    Route.AddStep(Board[0][0]);
}

CheckerBoard::CheckerBoard() {

}

void CheckerBoard::SetSize(int size) {
    Size = size;
    ReSetCheckerBoard();
}

void CheckerBoard::ReSetCheckerBoard() {
    Board = new ChessNode*[Size+2];
    for (int i = 0; i < Size+2; i++) {
        Board[i] = new ChessNode[Size+2];
    }
    for (int i = 1; i <= Size; i++) {
        for (int j = 1; j <= Size; j++) {
            Board[i][j].SetChess(i,j,-1,-1);
        }
    }
    Route.AddStep(Board[0][0]);
    Route.AddStep(Board[0][0]);
}

bool CheckerBoard::PlaceNode(int *location, int placeplayerid) {
    Board[location[0]][location[1]].SetStep(Route.RecentStep().STEPORDINAL()+1);
    Board[location[0]][location[1]].SetPlayer(placeplayerid);
    Route.AddStep(Board[location[0]][location[1]]);
    return Route.isChecked();
}

bool CheckerBoard::PlaceNode(int location_x, int location_y, int placeplayerid) {
    Board[location_x][location_y].SetStep(Route.RecentStep().STEPORDINAL()+1);
    Board[location_x][location_y].SetPlayer(placeplayerid);
    Route.AddStep(Board[location_x][location_y]);
    return Route.isChecked();
}

