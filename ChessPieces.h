#pragma once
#include <vector>
using namespace std;


class ChessPiece
{
    public:
        int pos [2] = {0, 0};
        vector<vector<int>> AllOffSets = {{0, 0}};
        vector<vector<int>> PossibleMoves = {{0, 0}};
        vector<vector<int>> boardValues = {{0}};
        ChessPiece(char symbol, bool isWhite) : symbol(symbol), isWhite(isWhite) {};
        char getSymbol() const { return symbol; };
        bool getIsWhite() const { return isWhite; };
        void setIsWhite(bool isWhite) { this->isWhite = isWhite; };
        bool getIsPinned() const { return isPinned; };
        void setIsPinned(bool isPinned) { this->isPinned = isPinned; };
        bool getIsPinning() const { return isPinning; };
        void setIsPinning(bool isPinning) { this->isPinning = isPinning; };
        bool getIsPinningKing() const { return isPinningKing; };
        void setIsPinningKing(bool isPinningKing) { this->isPinningKing = isPinningKing; };
        void InputOffsets(ChessPiece*);
        double getValue() const { return value; };
        void setValue(double value) { this->value = value; };
    private:
        double value;
        char symbol;
        bool isWhite;
        bool isPinned = false;
        bool isPinning = false;
        bool isPinningKing = false;
};
class Pawn : public ChessPiece {
public:
    Pawn(bool isWhite) : ChessPiece(isWhite ? 'P' : 'p', isWhite) {
        this->setValue(10);
        if(isWhite){
            this->boardValues = {{0, 0, 0, 0, 0, 0, 0, 0},
                                 {5, 10, 10, -20, -20, 10, 10, 5},
                                 {5, -5, -10, 0, 0, -10, -5, 5},
                                 {0, 0, 0, 20, 20, 0, 0, 0},
                                 {5, 5, 10, 25, 25, 10, 5, 5},
                                 {10, 10, 20, 30, 30, 20, 10, 10},
                                 {50, 50, 50, 50, 50, 50, 50, 50},
                                 {0, 0, 0, 0, 0, 0, 0, 0}};
        }
        else{
            this->boardValues = {{0, 0, 0, 0, 0, 0, 0, 0},
                                 {50, 50, 50, 50, 50, 50, 50, 50},
                                 {10, 10, 20, 30, 30, 20, 10, 10},
                                 {5, 5, 10, 25, 25, 10, 5, 5},
                                 {0, 0, 0, 20, 20, 0, 0, 0},
                                 {5, -5, -10, 0, 0, -10, -5, 5},
                                 {5, 10, 10, -20, -20, 10, 10, 5},
                                 {0, 0, 0, 0, 0, 0, 0, 0}};
        }
    }
};

class Rook : public ChessPiece {
public:
    Rook(bool isWhite) : ChessPiece(isWhite ? 'R' : 'r', isWhite) {
        this->setValue(50);
        if(isWhite){
            this->boardValues = {{0, 0, 0, 0, 0, 0, 0, 0},
                                 {5, 10, 10, 10, 10, 10, 10, 5},
                                 {-5, 0, 0, 0, 0, 0, 0, -5},
                                 {-5, 0, 0, 0, 0, 0, 0, -5},
                                 {-5, 0, 0, 0, 0, 0, 0, -5},
                                 {-5, 0, 0, 0, 0, 0, 0, -5},
                                 {-5, 0, 0, 0, 0, 0, 0, -5},
                                 {0, 0, 0, 5, 5, 0, 0, 0}};
        }else{
            this->boardValues = {{0, 0, 0, 5, 5, 0, 0, 0},
                                 {-5, 0, 0, 0, 0, 0, 0, -5},
                                 {-5, 0, 0, 0, 0, 0, 0, -5},
                                 {-5, 0, 0, 0, 0, 0, 0, -5},
                                 {-5, 0, 0, 0, 0, 0, 0, -5},
                                 {-5, 0, 0, 0, 0, 0, 0, -5},
                                 {5, 10, 10, 10, 10, 10, 10, 5},
                                 {0, 0, 0, 0, 0, 0, 0, 0}};
        }
    }
};

class Knight : public ChessPiece {
public:
    Knight(bool isWhite) : ChessPiece(isWhite ? 'N' : 'n', isWhite) {
        this->setValue(30);
        this->boardValues = {{-5, -4, -3, -3, -3, -3, -4, -5},
                             {-4, -2,  0,  0,  0,  0, -2, -4},
                             {-3,  0,  1,  1,  1,  1,  0, -3},
                             {-3,  1,  1,  2,  2,  1,  1, -3},
                             {-3,  0,  1,  2,  2,  1,  0, -3},
                             {-3,  1,  1,  1,  1,  1,  1, -3},
                             {-4, -2,  0,  1,  1,  0, -2, -4},
                             {-5, -4, -3, -3, -3, -3, -4, -5}};
    }
};

class Bishop : public ChessPiece {
public:
    Bishop(bool isWhite) : ChessPiece(isWhite ? 'B' : 'b', isWhite) {
        this->setValue(30);
        this->boardValues = {{-2, -1, -1, -1, -1, -1, -1, -2},
                             {-1,  0,  0,  0,  0,  0,  0, -1},
                             {-1,  0,  1,  1,  1,  1,  0, -1},
                             {-1,  1,  1,  1,  1,  1,  1, -1},
                             {-1,  0,  1,  1,  1,  1,  0, -1},
                             {-1,  1,  1,  1,  1,  1,  1, -1},
                             {-1,  1,  0,  0,  0,  0,  1, -1},
                             {-2, -1, -1, -1, -1, -1, -1, -2}};
    }
};

class Queen : public ChessPiece {
public:
    Queen(bool isWhite) : ChessPiece(isWhite ? 'Q' : 'q', isWhite) {
        this->setValue(90);
        if(isWhite){
            this->boardValues = {{-2, -1, -1, -1, -1, -1, -1, -2},
                                 {-1,  0,  0,  0,  0,  0,  0, -1},
                                 {-1,  0,  1,  1,  1,  1,  0, -1},
                                 {-1,  1,  1,  1,  1,  1,  1, -1},
                                 {-1,  0,  1,  1,  1,  1,  0, -1},
                                 {-1,  1,  1,  1,  1,  1,  1, -1},
                                 {-1,  1,  0,  0,  0,  0,  1, -1},
                                 {-2, -1, -1, -1, -1, -1, -1, -2}};
        }
        else{
            this->boardValues = {{-2, -1, -1, -1, -1, -1, -1, -2},
                                 {-1,  1,  0,  0,  0,  0,  1, -1},
                                 {-1,  1,  1,  1,  1,  1,  1, -1},
                                 {-1,  0,  1,  1,  1,  1,  0, -1},
                                 {-1,  1,  1,  1,  1,  1,  1, -1},
                                 {-1,  0,  1,  1,  1,  1,  0, -1},
                                 {-1,  0,  0,  0,  0,  0,  0, -1},
                                 {-2, -1, -1, -1, -1, -1, -1, -2}};
        }
    }

};

class King : public ChessPiece {
public:
    King(bool isWhite) : ChessPiece(isWhite ? 'K' : 'k', isWhite) {
        this->setValue(1000);
        if(isWhite){
            this->boardValues = {{2,  3,  1,  0,  0,  1,  3,  2},
                                 {2,  2,  0,  0,  0,  0,  2,  2},
                                 {-1, -2, -2, -2, -2, -2, -2, -1},
                                 {-2, -3, -3, -4, -4, -3, -3, -2},
                                 {-3, -4, -4, -5, -5, -4, -4, -3},
                                 {-3, -4, -4, -5, -5, -4, -4, -3},
                                 {-3, -4, -4, -5, -5, -4, -4, -3},
                                 {-3, -4, -4, -5, -5, -4, -4, -3}};
        }
        else{
            this->boardValues = {{-3, -4, -4, -5, -5, -4, -4, -3},
                                 {-3, -4, -4, -5, -5, -4, -4, -3},
                                 {-3, -4, -4, -5, -5, -4, -4, -3},
                                 {-3, -4, -4, -5, -5, -4, -4, -3},
                                 {-2, -3, -3, -4, -4, -3, -3, -2},
                                 {-1, -2, -2, -2, -2, -2, -2, -1},
                                 {2,  2,  0,  0,  0,  0,  2,  2},
                                 {2,  3,  1,  0,  0,  1,  3,  2}};

        }
    }
};

void ChessPiece::InputOffsets(ChessPiece *piece){

    
    if (piece->getSymbol() == 'P' || piece->getSymbol() == 'p'){
        if (!piece->getIsWhite()){
            piece->AllOffSets = { {-1, 0} , {-2, 0}, {-1, 1}, {-1, -1}};
        }
        else{
            piece->AllOffSets = { {1, 0} , {2, 0}, {1, 1}, {1, -1}};
        }
    }
    
    else if (piece->getSymbol() == 'R' || piece->getSymbol() == 'r'){
        if (!piece->getIsWhite()){
            piece->AllOffSets = { {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0},
                              {-6, 0}, {-7, 0}, {1, 0}, {2, 0}, {3, 0},
                              {4, 0}, {5, 0}, {6, 0}, {7, 0}, {0, -1},
                              {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6},
                              {0, -7}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
                              {0, 5}, {0, 6}, {0, 7} };
        }
        else{
            piece->AllOffSets = { {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0},
                              {6, 0}, {7, 0}, {-1, 0}, {-2, 0}, {-3, 0},
                              {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}, {0, 1},
                              {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6},
                              {0, 7}, {0, -1}, {0, -2}, {0, -3}, {0, -4},
                              {0, -5}, {0, -6}, {0, -7} };
        }
    }
    else if (piece->getSymbol() == 'N' || piece->getSymbol() == 'n'){
        if (!piece->getIsWhite()){
            piece->AllOffSets = { {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2},
                              {1, -2}, {2, -1}, {2, 1}, {1, 2} };
        }
        else{
            piece->AllOffSets = { {1, 2}, {2, 1}, {2, -1}, {1, -2},
                              {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2} };
        }
    }
    else if (piece->getSymbol() == 'B' || piece->getSymbol() == 'b'){
        if (!piece->getIsWhite()){
            piece->AllOffSets = { {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5},
                              {-6, -6}, {-7, -7}, {1, 1}, {2, 2}, {3, 3},
                              {4, 4}, {5, 5}, {6, 6}, {7, 7}, {-1, 1},
                              {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6},
                              {-7, 7}, {1, -1}, {2, -2}, {3, -3}, {4, -4},
                              {5, -5}, {6, -6}, {7, -7} };
        }
        else{
            piece->AllOffSets = { {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5},
                              {6, 6}, {7, 7}, {-1, -1}, {-2, -2}, {-3, -3},
                              {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}, {-1, 1},
                              {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6},
                              {-7, 7}, {1, -1}, {2, -2}, {3, -3}, {4, -4},
                              {5, -5}, {6, -6}, {7, -7} };
        }
    }
    else if (piece->getSymbol() == 'Q' || piece->getSymbol() == 'q'){
        if (!piece->getIsWhite()){
            piece->AllOffSets = { {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0},
                              {-6, 0}, {-7, 0}, {1, 0}, {2, 0}, {3, 0},
                              {4, 0}, {5, 0}, {6, 0}, {7, 0}, {0, -1},
                              {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6},
                              {0, -7}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
                              {0, 5}, {0, 6}, {0, 7}, {-1, -1}, {-2, -2},
                              {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6},
                              {-7, -7}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
                              {5, 5}, {6, 6}, {7, 7}, {-1, 1}, {-2, 2},
                              {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7},
                              {1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5},
                              {6, -6}, {7, -7} };
        }
        else{
            piece->AllOffSets = { {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0},
                              {6, 0}, {7, 0}, {-1, 0}, {-2, 0}, {-3, 0},
                              {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}, {0, 1},
                              {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6},
                              {0, 7}, {0, -1}, {0, -2}, {0, -3}, {0, -4},
                              {0, -5}, {0, -6}, {0, -7}, {1, 1}, {2, 2},
                              {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7},
                              {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4},
                              {-5, -5}, {-6, -6}, {-7, -7}, {-1, 1},
                              {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6},
                              {-7, 7}, {1, -1}, {2, -2}, {3, -3}, {4, -4},
                              {5, -5}, {6, -6}, {7, -7} };
        }
    }
    else if (piece->getSymbol() == 'K' || piece->getSymbol() == 'k'){
        if (!piece->getIsWhite()){
            piece->AllOffSets = { {-1, 0}, {-1, -1}, {0, -1}, {1, -1},
                              {1, 0}, {1, 1}, {0, 1}, {-1, 1} };
        }
        else{
            piece->AllOffSets = { {1, 0}, {1, 1}, {0, 1}, {-1, 1},
                              {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };
        }
    }
}
