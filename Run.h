#pragma once
#include "ChessBoard.h"
#include <iostream>
#include "ChessAI.h"
#include <limits>
#include <fstream>
#include <unordered_map>
#include "json.hpp"
using namespace std;
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define CLEAR "\033[2J\033[1;1H"

using json = nlohmann::json; // Alias for convenience

bool HasTheKingBeenEaten(ChessBoard&);
bool CheckIfMoveIsPossible(ChessPiece*, int, int);
bool checkIfPlayerPickedWhite(int, int, ChessBoard&);
void test(ChessAI, ChessBoard&);
void test2(ChessAI, ChessBoard&);
void test3(ChessAI, ChessBoard&);
void runGame();
void saveMapToJsonFile(const unordered_map<string, double>&, const string&);
unordered_map<string, double> loadMapFromJsonFile(const string&);
int CountPieces(ChessBoard&);
bool hasntEatenFor50Rounds = false;

bool PlayerTurn = true;

void runGame(){
    ChessBoard board;
    ChessAI AI;
    ChessAI AI2;
    double score = 0;
    int numberOfPieces = 32;
    int roundsWithoutCapture = 0;
    // test3(AI, board);
    // return;
    //cout << AI.EvaluateBoard(board);
    //read the BoardScoresWhite and BoardScoresBlack maps from a file
    try {
    // Load maps
    AI.BoardScoresWhite = loadMapFromJsonFile("BoardScoresWhite.json");
    AI.BoardScoresBlack = loadMapFromJsonFile("BoardScoresBlack.json");
    //cout << AI.BoardScoresBlack.size() << endl;
    //cout << AI.BoardScoresWhite.size() << endl;

        // Now you have loadedWhite and loadedBlack maps ready to use
    } 
    catch (const json::parse_error& e) {
        cout << "JSON parse error: " << e.what() << endl;
    } 
    catch (const json::type_error& e) {
        cout << "JSON type error: " << e.what() << endl;
    } 
    catch (const std::exception& e) {
        cout << "Error occurred: " << e.what() << endl;
    }

    
    while(!HasTheKingBeenEaten(board)){
        if(roundsWithoutCapture == 50){
            hasntEatenFor50Rounds = true;
            cout << "50 rounds without capture" << endl;
            cout << "press enter to exit" << endl;
            cin.ignore();
            cin.get();
            break;
        }
         if(PlayerTurn){
            system("clear");
            cout << "Player's turn" << endl;
            board.printBoardForWhite();
            cout << BLUE;
            WrongMove:
            cout << "Enter the row coordinates of the piece you want to move: ";
            int x1;
            cin >> x1;
            cout << "Enter the column coordinates of the piece you want to move: ";
            int y1;
            cin >> y1;
            if(x1 > 7 || x1 < 0 || y1 > 7 || y1 < 0){
                cout << "This is not a valid location" << endl;
                goto WrongMove;
            }
            if(board.board[x1][y1] == NULL){
                cout << "There is no piece at this location" << endl;
                goto WrongMove;
            }
            if(!checkIfPlayerPickedWhite(x1, y1, board)){
                cout << "You picked a black piece" << endl;
                goto WrongMove;
            }
            board.CalculatePossibleMoves(board.board[x1][y1], board);
            if(board.board[x1][y1]->PossibleMoves.size() == 0){
                cout << "There are no possible moves for this piece" << endl;
                // go back to Rawad
                goto WrongMove;
            }
            else{
            cout << "The possible moves for " << board.board[x1][y1]->getSymbol() << " are: " << endl;
            }
            for(int i = 0; i < board.board[x1][y1]->PossibleMoves.size(); i++){
                cout << board.board[x1][y1]->PossibleMoves[i][0] << " " << board.board[x1][y1]->PossibleMoves[i][1] << endl;
            }
            cout << "Enter the row coordinates of the place you want to move the piece to: ";
            int x2;
            cin >> x2;
            cout << "Enter the column coordinates of the place you want to move the piece to: ";
            int y2;
            cin >> y2;
            if(x2 > 7 || x2 < 0 || y2 > 7 || y2 < 0){
                cout << "This is not a valid location" << endl;
                goto WrongMove;
            }
            if(!CheckIfMoveIsPossible(board.board[x1][y1], x2, y2)){
                cout << "This move is not possible" << endl;
                goto WrongMove;
            }
            board.MovePieceForPlayer(board.board[x1][y1], x2, y2);
            if(board.board[x2][y2]->getSymbol()=='P' && x2==7){
                char symbol;
                cout << "Enter the symbol you want to upgrade your pawn to: ";
                cin >> symbol;
                board.UpgradePawn(board.board[x2][y2], symbol);
            }
            PlayerTurn = false;
            cout << RESET;
            // cout << "AI2's turn" << endl;
            // board.printBoardForWhite();
            // AI2.ChildFactoryWhite(board, AI2.boardQueueWhite);
            // AI2.DFSWhite(AI2.boardQueueWhite, INT_MIN, INT_MAX);
            // score = AI2.boardQueueWhite.peek().first;
            // board = AI2.boardQueueWhite.poll().second;
            // AI2.boardQueueWhite.clear();
            // PlayerTurn = false;
            // cout << RESET;
        }
        else{
            system("clear");
            cout << "AI's turn" << endl;
            board.printBoardForWhite();
            AI.ChildFactoryBlack(board, AI.boardQueue);
            AI.LoopThrough(AI.boardQueue, INT_MIN, INT_MAX);
            score = AI.boardQueue.peek().first;
            board = AI.boardQueue.poll().second;
            AI.boardQueue.clear();
            PlayerTurn = true;
        }
        int newNumberOfPieces = CountPieces(board);
        if(newNumberOfPieces == numberOfPieces){
            roundsWithoutCapture++;
        }
        else{
            numberOfPieces = newNumberOfPieces;
            roundsWithoutCapture = 0;
        }
    }
    //save the BoardScoresWhite and BoardScoresBlack maps to a file
    saveMapToJsonFile(AI.BoardScoresWhite, "BoardScoresWhite.json");
    saveMapToJsonFile(AI.BoardScoresBlack, "BoardScoresBlack.json");

    if(hasntEatenFor50Rounds){
        cout << "50 rounds without capture" << endl;
        cout << "the winner is: " << endl;
        if(score > 0){
            cout << "White" << endl;
        }
        else{
            cout << "Black" << endl;
        }
    }

    if(board.getWhiteWon()){
        cout << "White won" << endl;
        cout << "press enter to exit" << endl;
        cin.ignore();
        cin.get();
    }
    else{
        cout << "Black won" << endl;
        cout << "press enter to exit" << endl;
        cin.ignore();
        cin.get();
    }
    
}

int CountPieces(ChessBoard& board){
    int count = 0;
    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            if(board.board[i][j] != NULL){
                count++;
            }
        }
    }
    return count;
}




void saveMapToJsonFile(const unordered_map<string, double>& mapToSave, const string& filename) {
    json mapJson(mapToSave);

    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << mapJson.dump();
        cout << "Map saved to file: " << filename << endl;
    } else {
        cout << "Unable to open file for writing: " << filename << endl;
    }
    outFile.close();
}

unordered_map<string, double> loadMapFromJsonFile(const string& filename) {
    ifstream inFile(filename);
    unordered_map<string, double> loadedMap;

    if (inFile.is_open()) {
        json loadedJson;
        inFile >> loadedJson;

        loadedMap = loadedJson.get<unordered_map<string, double>>();
        cout << "Map loaded from file: " << filename << endl;
    } else {
        cout << "File doesn't exist: " << filename << endl;
    }
    inFile.close();

    return loadedMap;
}

void test(ChessAI AI, ChessBoard& board){
    //nullify the board
    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            board.board[i][j] = NULL;
        }
    }
    //set the pieces
    board.board[0][0] = new Rook(true);
    board.board[0][0]->pos[0] = 0;
    board.board[0][0]->pos[1] = 0;
    board.board[0][0]->InputOffsets(board.board[0][0]);
    board.board[0][3] = new King(true);
    board.board[0][3]->pos[0] = 0;
    board.board[0][3]->pos[1] = 3;
    board.board[0][3]->InputOffsets(board.board[0][3]);
    board.board[0][4] = new Queen(true);
    board.board[0][4]->pos[0] = 0;
    board.board[0][4]->pos[1] = 4;
    board.board[0][4]->InputOffsets(board.board[0][4]);
    board.board[0][5] = new Bishop(true);
    board.board[0][5]->pos[0] = 0;
    board.board[0][5]->pos[1] = 5;
    board.board[0][5]->InputOffsets(board.board[0][5]);
    board.board[0][6] = new Knight(true);
    board.board[0][6]->pos[0] = 0;
    board.board[0][6]->pos[1] = 6;
    board.board[0][6]->InputOffsets(board.board[0][6]);
    board.board[0][7] = new Rook(true);
    board.board[0][7]->pos[0] = 0;
    board.board[0][7]->pos[1] = 7;
    board.board[0][7]->InputOffsets(board.board[0][7]);
    board.board[1][0] = new Pawn(true);
    board.board[1][0]->pos[0] = 1;
    board.board[1][0]->pos[1] = 0;
    board.board[1][0]->InputOffsets(board.board[1][0]);
    board.board[1][1] = new Pawn(true);
    board.board[1][1]->pos[0] = 1;
    board.board[1][1]->pos[1] = 1;
    board.board[1][1]->InputOffsets(board.board[1][1]);
    board.board[1][2] = new Knight(false);
    board.board[1][2]->pos[0] = 1;
    board.board[1][2]->pos[1] = 2;
    board.board[1][2]->InputOffsets(board.board[1][2]);
    board.board[1][3] = new Knight(true);
    board.board[1][3]->pos[0] = 1;
    board.board[1][3]->pos[1] = 3;
    board.board[1][3]->InputOffsets(board.board[1][3]);
    board.board[1][5] = new Pawn(true);
    board.board[1][5]->pos[0] = 1;
    board.board[1][5]->pos[1] = 5;
    board.board[1][5]->InputOffsets(board.board[1][5]);
    board.board[1][6] = new Pawn(true);
    board.board[1][6]->pos[0] = 1;
    board.board[1][6]->pos[1] = 6;
    board.board[1][6]->InputOffsets(board.board[1][6]);
    board.board[1][7] = new Pawn(true);
    board.board[1][7]->pos[0] = 1;
    board.board[1][7]->pos[1] = 7;
    board.board[1][7]->InputOffsets(board.board[1][7]);
    board.board[3][5] = new Bishop(true);
    board.board[3][5]->pos[0] = 3;
    board.board[3][5]->pos[1] = 5;
    board.board[3][5]->InputOffsets(board.board[3][5]);
    board.board[5][5] = new Bishop(false);
    board.board[5][5]->pos[0] = 5;
    board.board[5][5]->pos[1] = 5;
    board.board[5][5]->InputOffsets(board.board[5][5]);
    board.board[6][0] = new Pawn(false);
    board.board[6][0]->pos[0] = 6;
    board.board[6][0]->pos[1] = 0;
    board.board[6][0]->InputOffsets(board.board[6][0]);
    board.board[6][1] = new Pawn(false);
    board.board[6][1]->pos[0] = 6;
    board.board[6][1]->pos[1] = 1;
    board.board[6][1]->InputOffsets(board.board[6][1]);
    board.board[6][2] = new Pawn(false);
    board.board[6][2]->pos[0] = 6;
    board.board[6][2]->pos[1] = 2;
    board.board[6][2]->InputOffsets(board.board[6][2]);
    board.board[6][3] = new Bishop(false);
    board.board[6][3]->pos[0] = 6;
    board.board[6][3]->pos[1] = 3;
    board.board[6][3]->InputOffsets(board.board[6][3]);
    board.board[6][4] = new Pawn(true);
    board.board[6][4]->pos[0] = 6;
    board.board[6][4]->pos[1] = 4;
    board.board[6][4]->InputOffsets(board.board[6][4]);
    board.board[6][6] = new Pawn(false);
    board.board[6][6]->pos[0] = 6;
    board.board[6][6]->pos[1] = 6;
    board.board[6][6]->InputOffsets(board.board[6][6]);
    board.board[6][7] = new Pawn(false);
    board.board[6][7]->pos[0] = 6;
    board.board[6][7]->pos[1] = 7;
    board.board[6][7]->InputOffsets(board.board[6][7]);
    board.board[7][0] = new Rook(false);
    board.board[7][0]->pos[0] = 7;
    board.board[7][0]->pos[1] = 0;
    board.board[7][0]->InputOffsets(board.board[7][0]);
    board.board[7][2] = new King(false);
    board.board[7][2]->pos[0] = 7;
    board.board[7][2]->pos[1] = 2;
    board.board[7][2]->InputOffsets(board.board[7][2]);
    board.board[7][4] = new Queen(false);   
    board.board[7][4]->pos[0] = 7;
    board.board[7][4]->pos[1] = 4;
    board.board[7][4]->InputOffsets(board.board[7][4]);
    board.board[7][5] = new Bishop(false);
    board.board[7][5]->pos[0] = 7;
    board.board[7][5]->pos[1] = 5;
    board.board[7][5]->InputOffsets(board.board[7][5]);
    board.board[7][6] = new Knight(false);
    board.board[7][6]->pos[0] = 7;
    board.board[7][6]->pos[1] = 6;
    board.board[7][6]->InputOffsets(board.board[7][6]);
    board.board[7][7] = new Rook(false);
    board.board[7][7]->pos[0] = 7;
    board.board[7][7]->pos[1] = 7;
    board.board[7][7]->InputOffsets(board.board[7][7]);

    board.printBoardForWhite();

    // AI.ChildFactoryBlack(board, AI.boardQueue);
    // //AI.ChildFactoryWhite(board, AI.boardQueue);
    
    // cout << "Number of possible boards: " << AI.boardQueue.size() << endl;
    // AI.DFSBlack(AI.boardQueue, -2147483647, 2147483647);
    // double score = AI.boardQueue.peek().first;
    // cout << AI.possibleBoards << endl;
    // AI.boardQueue.clear();
}

void test2(ChessAI AI, ChessBoard& board){
    double score = AI.EvaluateBoard(board);
    cout << score << endl;
}

void test3(ChessAI AI, ChessBoard& board){
    double score = 0;
    //nullify the board
    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            board.board[i][j] = NULL;
        }
    }

    board.board[7][1] = new Rook(false);
    board.board[7][1]->pos[0] = 7;
    board.board[7][1]->pos[1] = 1;
    board.board[7][1]->InputOffsets(board.board[7][1]);
    board.board[7][2] = new Bishop(false);
    board.board[7][2]->pos[0] = 7;
    board.board[7][2]->pos[1] = 2;
    board.board[7][2]->InputOffsets(board.board[7][2]);
    board.board[7][3] = new King(false);
    board.board[7][3]->pos[0] = 7;
    board.board[7][3]->pos[1] = 3;
    board.board[7][3]->InputOffsets(board.board[7][3]);
    board.board[7][5] = new Bishop(false);
    board.board[7][5]->pos[0] = 7;
    board.board[7][5]->pos[1] = 5;
    board.board[7][5]->InputOffsets(board.board[7][5]);
    board.board[7][7] = new Rook(false);
    board.board[7][7]->pos[0] = 7;
    board.board[7][7]->pos[1] = 7;
    board.board[7][7]->InputOffsets(board.board[7][7]);
    board.board[6][0] = new Pawn(false);
    board.board[6][0]->pos[0] = 6;
    board.board[6][0]->pos[1] = 0;
    board.board[6][0]->InputOffsets(board.board[6][0]);
    board.board[6][1] = new Pawn(false);
    board.board[6][1]->pos[0] = 6;
    board.board[6][1]->pos[1] = 1;
    board.board[6][1]->InputOffsets(board.board[6][1]);
    board.board[6][3] = new Pawn(false);
    board.board[6][3]->pos[0] = 6;
    board.board[6][3]->pos[1] = 3;
    board.board[6][3]->InputOffsets(board.board[6][3]);
    board.board[6][5] = new Pawn(false);
    board.board[6][5]->pos[0] = 6;
    board.board[6][5]->pos[1] = 5;
    board.board[6][5]->InputOffsets(board.board[6][5]);
    board.board[6][7] = new Pawn(false);
    board.board[6][7]->pos[0] = 6;
    board.board[6][7]->pos[1] = 7;
    board.board[6][7]->InputOffsets(board.board[6][7]);
    board.board[5][7] = new Queen(false);
    board.board[5][7]->pos[0] = 5;
    board.board[5][7]->pos[1] = 7;
    board.board[5][7]->InputOffsets(board.board[5][7]);
    board.board[5][5] = new Pawn(false);
    board.board[5][5]->pos[0] = 5;
    board.board[5][5]->pos[1] = 5;
    board.board[5][5]->InputOffsets(board.board[5][5]);
    board.board[5][4] = new Pawn(false);
    board.board[5][4]->pos[0] = 5;
    board.board[5][4]->pos[1] = 4;
    board.board[5][4]->InputOffsets(board.board[5][4]);
    board.board[3][3] = new Knight(false);
    board.board[3][3]->pos[0] = 3;
    board.board[3][3]->pos[1] = 3;
    board.board[3][3]->InputOffsets(board.board[3][3]);
    board.board[3][1] = new Pawn(true);
    board.board[3][1]->pos[0] = 3;
    board.board[3][1]->pos[1] = 1;
    board.board[3][1]->InputOffsets(board.board[3][1]);
    board.board[2][7] = new Knight(true);
    board.board[2][7]->pos[0] = 2;
    board.board[2][7]->pos[1] = 7;
    board.board[2][7]->InputOffsets(board.board[2][7]);
    board.board[2][6] = new Pawn(true);
    board.board[2][6]->pos[0] = 2;
    board.board[2][6]->pos[1] = 6;
    board.board[2][6]->InputOffsets(board.board[2][6]);
    board.board[2][5] = new Knight(false);
    board.board[2][5]->pos[0] = 2;
    board.board[2][5]->pos[1] = 5;
    board.board[2][5]->InputOffsets(board.board[2][5]);
    board.board[2][4] = new Pawn(true);
    board.board[2][4]->pos[0] = 2;
    board.board[2][4]->pos[1] = 4;
    board.board[2][4]->InputOffsets(board.board[2][4]);
    board.board[2][0] = new Pawn(true);
    board.board[2][0]->pos[0] = 2;
    board.board[2][0]->pos[1] = 0;
    board.board[2][0]->InputOffsets(board.board[2][0]);
    board.board[1][7] = new Pawn(true);
    board.board[1][7]->pos[0] = 1;
    board.board[1][7]->pos[1] = 7;
    board.board[1][7]->InputOffsets(board.board[1][7]);
    board.board[1][5] = new Pawn(true);
    board.board[1][5]->pos[0] = 1;
    board.board[1][5]->pos[1] = 5;
    board.board[1][5]->InputOffsets(board.board[1][5]);
    board.board[1][4] = new King(true);
    board.board[1][4]->pos[0] = 1;
    board.board[1][4]->pos[1] = 4;
    board.board[1][4]->InputOffsets(board.board[1][4]);
    board.board[1][2] = new Pawn(true); 
    board.board[1][2]->pos[0] = 1;
    board.board[1][2]->pos[1] = 2;
    board.board[1][2]->InputOffsets(board.board[1][2]);
    board.board[1][1] = new Rook(true);
    board.board[1][1]->pos[0] = 1;
    board.board[1][1]->pos[1] = 1;
    board.board[1][1]->InputOffsets(board.board[1][1]);
    board.board[0][7] = new Rook(true);


    // board.board[0][3] = new King(false);
    // board.board[0][3]->pos[0] = 0;
    // board.board[0][3]->pos[1] = 3;
    // board.board[0][3]->InputOffsets(board.board[0][3]);
    // board.board[3][3] = new Queen(true);
    // board.board[3][3]->pos[0] = 3;
    // board.board[3][3]->pos[1] = 3;
    // board.board[3][3]->InputOffsets(board.board[3][3]);

    // board.board[4][3] = new King(true);
    // board.board[4][3]->pos[0] = 4;
    // board.board[4][3]->pos[1] = 3;
    // board.board[4][3]->InputOffsets(board.board[4][3]);

    while(!HasTheKingBeenEaten(board)){
        if(PlayerTurn){
            board.printBoardForWhite();
            WrongMove:
            cout << "Enter the row coordinates of the piece you want to move: ";
            int x1;
            cin >> x1;
            cout << "Enter the column coordinates of the piece you want to move: ";
            int y1;
            cin >> y1;
            if(x1 > 7 || x1 < 0 || y1 > 7 || y1 < 0){
                cout << "This is not a valid location" << endl;
                goto WrongMove;
            }
            if(board.board[x1][y1] == NULL){
                cout << "There is no piece at this location" << endl;
                goto WrongMove;
            }
            if(!checkIfPlayerPickedWhite(x1, y1, board)){
                cout << "You picked a black piece" << endl;
                goto WrongMove;
            }
            board.CalculatePossibleMoves(board.board[x1][y1], board);
            if(board.board[x1][y1]->PossibleMoves.size() == 0){
                cout << "There are no possible moves for this piece" << endl;
                // go back to Rawad
                goto WrongMove;
            }
            else{
            cout << "The possible moves for " << board.board[x1][y1]->getSymbol() << " are: " << endl;
            }
            for(int i = 0; i < board.board[x1][y1]->PossibleMoves.size(); i++){
                cout << board.board[x1][y1]->PossibleMoves[i][0] << " " << board.board[x1][y1]->PossibleMoves[i][1] << endl;
            }
            cout << "Enter the row coordinates of the place you want to move the piece to: ";
            int x2;
            cin >> x2;
            cout << "Enter the column coordinates of the place you want to move the piece to: ";
            int y2;
            cin >> y2;
            if(x2 > 7 || x2 < 0 || y2 > 7 || y2 < 0){
                cout << "This is not a valid location" << endl;
                goto WrongMove;
            }
            if(!CheckIfMoveIsPossible(board.board[x1][y1], x2, y2)){
                cout << "This move is not possible" << endl;
                goto WrongMove;
            }
            board.MovePieceForPlayer(board.board[x1][y1], x2, y2);
            if(board.board[x2][y2]->getSymbol()=='P' && x2==7){
                char symbol;
                cout << "Enter the symbol you want to upgrade your pawn to: ";
                cin >> symbol;
                board.UpgradePawn(board.board[x2][y2], symbol);
            }
            PlayerTurn = false;
        }
        else
        {
            // cout << "AI's turn" << endl;
            // board.printBoardForWhite();
            // AI.ChildFactoryBlack(board, AI.boardQueue);
            // AI.DFSBlack(AI.boardQueue, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
            // score = AI.boardQueue.peek().first;
            // board = AI.boardQueue.poll().second;
            // AI.boardQueue.clear();
            // PlayerTurn = true;
        }
    }

}


bool HasTheKingBeenEaten(ChessBoard& board){
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
        board.printBoardForWhite();
        if(BlackKingCount == 0){
            board.setWhiteWon(true);
        }
        else{
            board.setWhiteWon(false);
        }
        return true;
    }
}
//function to check if the move inputted is within the possible moves of the piece
bool CheckIfMoveIsPossible(ChessPiece *piece, int x2, int y2){
    for(int i = 0; i < piece->PossibleMoves.size(); i++){
        if(piece->PossibleMoves[i][0] == x2 && piece->PossibleMoves[i][1] == y2){
            return true;
        }
    }
    return false;
}

bool checkIfPlayerPickedWhite(int x, int y, ChessBoard& board){
    if(board.board[x][y]->getIsWhite()){
        return true;
    }
    else{
        return false;
    }
}
