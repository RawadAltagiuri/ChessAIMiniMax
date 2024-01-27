#pragma once
#include <unordered_map>
#include "ChessBoard.h"
#include "ChessPieces.h"
#include <iostream>
#include "PDQquickMode.h"
#include <limits>


using namespace std;

class ChessAI{
    public:
        int possibleBoards = 0;
        unordered_map<string, double> BoardScoresWhite;
        unordered_map<string, double> BoardScoresBlack;
        ChessAI(){this->boardQueue = PDQ<std::pair<double,ChessBoard>>();};
        void ChildFactoryBlack(ChessBoard&, PDQ<std::pair<double,ChessBoard>>&);
        void ChildFactoryWhite(ChessBoard&, PDQ<std::pair<double,ChessBoard>>&);
        double EvaluateBoard(ChessBoard&);
        PDQ<std::pair<double,ChessBoard>> boardQueue;
        int getDepth(){return this->depth;};
        void setDepth(int depth){this->depth = depth;};
        double minxmax(std::pair<double,ChessBoard>, int, double, double, bool);
        void LoopThrough(PDQ<std::pair<double,ChessBoard>>&, double, double);
        bool HasTheKingBeenEaten(ChessBoard&);
        bool HasTheWhiteKingBeenEaten(ChessBoard&);
        bool HasTheBlackKingBeenEaten(ChessBoard&); 

    private:
        int depth = 3;
};

void ChessAI::ChildFactoryBlack(ChessBoard& board, PDQ<std::pair<double,ChessBoard>>& boardQueue){
    boardQueue = PDQ<std::pair<double,ChessBoard>>();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            ChessPiece* piece = board.board[i][j];
            if(piece != NULL){
                if(!piece->getIsWhite()){
                    board.CalculatePossibleMoves(piece, board);
                    for(int k = 0; k < piece->PossibleMoves.size(); k++){
                        ChessBoard newBoard;
                        board.DeepCopy(board, newBoard);
                        newBoard.MovePieceForAI(newBoard.board[i][j], piece->PossibleMoves[k][0], piece->PossibleMoves[k][1]);
                        if(piece->getSymbol() == 'p'){
                            char symbol = 'q';
                            board.UpgradePawn(newBoard.board[piece->PossibleMoves[k][0]][piece->PossibleMoves[k][1]], symbol);
                        }
                        double score = EvaluateBoard(newBoard);
                        boardQueue.add(std::make_pair(score, newBoard));
                    }
                }
            }
        }
    }
    int j = 0;
}

void ChessAI::ChildFactoryWhite(ChessBoard& board, PDQ<std::pair<double,ChessBoard>>& boardQueue){
    boardQueue = PDQ<std::pair<double,ChessBoard>>(true);
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            ChessPiece* piece = board.board[i][j];
            if(piece != NULL){
                if(piece->getIsWhite()){
                    board.CalculatePossibleMoves(piece, board);
                    for(int k = 0; k < piece->PossibleMoves.size(); k++){
                        ChessBoard newBoard;
                        board.DeepCopy(board, newBoard);
                        newBoard.MovePieceForAI(newBoard.board[i][j], piece->PossibleMoves[k][0], piece->PossibleMoves[k][1]);
                        char symbol = 'Q';
                        board.UpgradePawn(newBoard.board[piece->PossibleMoves[k][0]][piece->PossibleMoves[k][1]], symbol);
                        double score = EvaluateBoard(newBoard);
                        boardQueue.add(std::make_pair(score, newBoard));
                    }
                }
            }
        }
    }
}

double ChessAI::EvaluateBoard(ChessBoard& board){
    double score = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            ChessPiece* piece = board.board[i][j];
            if(piece != NULL){
                board.CalculatePossibleMoves(piece, board);
                board.IsPiecePinned(piece, board);
                board.IsPiecePinning(piece, board);
                double pieceValue = piece->getValue();
                pieceValue += piece->boardValues[i][j];
                if(piece->getIsWhite()){
                    if(piece->getSymbol()== 'K'){
                        if(HasTheKingBeenEaten(board)){
                            score = std::numeric_limits<double>::infinity();
                            return score;
                        }
                    }
                    if(piece->getIsPinned()){
                        pieceValue *= 0.5;
                    }
                    else if(piece->getIsPinning()){
                        pieceValue *= 2;
                    }
                    score += pieceValue;
                }
                else{
                    if(piece->getSymbol()== 'k'){
                        if(HasTheKingBeenEaten(board)){
                            score = std::numeric_limits<double>::infinity() * -1;
                            return score;
                        }
                    }
                    if(piece->getIsPinned()){
                        pieceValue *= 0.5;
                    }
                    else if(piece->getIsPinning()){
                        pieceValue *= 2;
                    }
                    score -= pieceValue;
                }
            }
        }
    }
    return score;
}



void ChessAI::LoopThrough(PDQ<std::pair<double,ChessBoard>>& boardQueue, double alpha, double beta) {
    double minEval = std::numeric_limits<double>::infinity();
    PDQ<std::pair<double,ChessBoard>> tempboardQueue = boardQueue;
    boardQueue = PDQ<std::pair<double,ChessBoard>>();
    int queueSize = tempboardQueue.size();

    for (int i = 0; i < queueSize; i++) {
        std::pair<double,ChessBoard> pair = tempboardQueue.poll();
        double lowestScore = this->minxmax(pair, this->depth, alpha, beta, true);
        minEval = min(minEval, lowestScore);
        beta = min(beta, lowestScore);
        boardQueue.add(std::make_pair(lowestScore, pair.second));
        if(beta <= alpha){
            break;
        }
        cout << "Still running" << endl;
    }
}

double ChessAI::minxmax(std::pair<double,ChessBoard> parent, int depth, double alpha, double beta, bool isMaximizingPlayer) {
    if (depth == 0 || this->HasTheKingBeenEaten(parent.second)) {
        double score = this->EvaluateBoard(parent.second);
        this->possibleBoards++;
        return score;
    }

    if(!isMaximizingPlayer){
        double minEval = std::numeric_limits<double>::infinity();
        ChessBoard board;
        parent.second.DeepCopy(parent.second, board);
        PDQ<std::pair<double,ChessBoard>> boardQueue(false);
        this->ChildFactoryBlack(board, boardQueue);
        int queueSize = boardQueue.size();
        for (int i = 0; i < queueSize; i++) {
            std::pair<double,ChessBoard> pair = boardQueue.poll();
            double score = this->minxmax(pair, depth - 1, alpha, beta, true);
            minEval = min(minEval, score);
            beta = min(beta, minEval);
            if(beta <= alpha){
                break;
            }
        }
        return minEval;
    }
    else{
        double maxEval = numeric_limits<double>::infinity() * -1;
        ChessBoard board;
        parent.second.DeepCopy(parent.second, board);
        PDQ<std::pair<double,ChessBoard>> boardQueue(true);
        this->ChildFactoryWhite(board, boardQueue);
        int queueSize = boardQueue.size();
        vector<double> scores;
        for (int i = 0; i < queueSize; i++) {
            std::pair<double,ChessBoard> pair = boardQueue.poll();
            double score = this->minxmax(pair, depth - 1, alpha, beta, false);
            maxEval = max(maxEval, score);
            alpha = max(alpha, maxEval);
            if(beta <= alpha){
                break;
            }
        }
        return maxEval;
    }
}


bool ChessAI::HasTheKingBeenEaten(ChessBoard& board){
    int BlackKingCount = 0;
    int WhiteKingCount = 0;
    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            if(board.board[i][j] != NULL){
                if(board.board[i][j]->getSymbol() == 'k'){
                    BlackKingCount++;
                }
                else if(board.board[i][j]->getSymbol() == 'K'){
                    WhiteKingCount++;
                }
            }
        }
    }
    int kingCount = BlackKingCount + WhiteKingCount;
    if(kingCount == 2){
        return false;
    }
    else{
        return true;
    }
}

bool ChessAI::HasTheWhiteKingBeenEaten(ChessBoard& board){
    int WhiteKingCount = 0;
    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            if(board.board[i][j] != NULL){
                if(board.board[i][j]->getSymbol() == 'K'){
                    WhiteKingCount++;
                }
            }
        }
    }
    if(WhiteKingCount == 1){
        return false;
    }
    else{
        return true;
    }
}

bool ChessAI::HasTheBlackKingBeenEaten(ChessBoard& board){
    int BlackKingCount = 0;
    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            if(board.board[i][j] != NULL){
                if(board.board[i][j]->getSymbol() == 'k'){
                    BlackKingCount++;
                }
            }
        }
    }
    if(BlackKingCount == 1){
        return false;
    }
    else{
        return true;
    }
}


