#pragma once
#include <vector>
#include "ChessPieces.h"
using namespace std;
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"



class ChessBoard
{
public:
    vector<vector<ChessPiece*>> board;
    ChessBoard();
    ~ChessBoard();
    void DeepCopy(ChessBoard& board, ChessBoard& newBoard);
    void printBoard();
    void printBoardForWhite();
    void setBoard();
    void CalculatePossibleMoves(ChessPiece *piece, ChessBoard& board);
    void MovePieceForAI(ChessPiece *piece, int x, int y);
    void MovePieceForPlayer(ChessPiece *piece, int x, int y);
    void UpgradePawn(ChessPiece *piece, char symbol);
    bool IsThereAPieceBetween(ChessPiece *piece, int x, int y);
    bool IsPiecePinned(ChessPiece *piece, ChessBoard& board);
    bool IsPiecePinning(ChessPiece *piece, ChessBoard& board);
    void setWhiteWon(bool WhiteWon) { this->WhiteWon = WhiteWon; };
    bool getWhiteWon() const { return WhiteWon; };
    string TurnBoardToString();
    private:
    bool WhiteWon;
};


ChessBoard::ChessBoard()
{
    this->board = vector<vector<ChessPiece*>>(8, vector<ChessPiece*>(8, NULL));
    this->setBoard();
}

ChessBoard::~ChessBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < this->board[i].size(); j++)
        {
            this->board[i][j] = NULL;
        }
        this->board[i].clear();
    }
}

void ChessBoard::printBoard()
{
    for (int i = 0; i < 8; i++)
    {
        cout << "  ---------------------------------\n" << endl;
        cout << i << " ";
        for (int j = 0; j < 8; j++)
        {
            if (this->board[i][j] == NULL)
            {
                cout << "|   ";
            }
            else
            {
                cout << "| " << this->board[i][j]->getSymbol() << " ";
            }
        }
        cout << "|" << endl;
    }
    cout << "  ---------------------------------" << endl;
    cout << "    0   1   2   3   4   5   6   7   " << endl << endl;
}

void ChessBoard::printBoardForWhite()
{
    for (int i = 7; i >= 0; i--)
    {
        cout << " ---------------------------------\n" << endl;
        cout << i << " ";
        for (int j = 7; j >= 0; j--)
        {
            if (this->board[i][j] == NULL)
            {
                cout << "|   ";
            }
            else
            {
                cout << "| "; this->board[i][j]->getIsWhite()==1 ? cout << GREEN : cout << RED;
                cout<<this->board[i][j]->getSymbol() << RESET <<" ";
            }
        }
        cout << "|" << endl;
    }
    cout << "  ---------------------------------" << endl;
    cout << "    7   6   5   4   3   2   1   0   " << endl << endl;
}

void ChessBoard::setBoard()
{
    for (int i = 0; i < 8; i++)
    {
        this->board[1][i] = new Pawn(true);
        this->board[1][i]->pos[0] = 1;
        this->board[1][i]->pos[1] = i;
        this->board[1][i]->InputOffsets(this->board[1][i]);

        this->board[6][i] = new Pawn(false);
        this->board[6][i]->pos[0] = 6;
        this->board[6][i]->pos[1] = i;
        this->board[6][i]->InputOffsets(this->board[6][i]);
    }

    this->board[0][0] = new Rook(true);
    this->board[0][0]->pos[0] = 0;
    this->board[0][0]->pos[1] = 0;
    this->board[0][0]->InputOffsets(this->board[0][0]);
    this->board[0][7] = new Rook(true);
    this->board[0][7]->pos[0] = 0;
    this->board[0][7]->pos[1] = 7;
    this->board[0][7]->InputOffsets(this->board[0][7]);
    this->board[7][0] = new Rook(false);
    this->board[7][0]->pos[0] = 7;
    this->board[7][0]->pos[1] = 0;
    this->board[7][0]->InputOffsets(this->board[7][0]);
    this->board[7][7] = new Rook(false);
    this->board[7][7]->pos[0] = 7;
    this->board[7][7]->pos[1] = 7;
    this->board[7][7]->InputOffsets(this->board[7][7]);

    this->board[0][1] = new Knight(true);
    this->board[0][1]->pos[0] = 0;
    this->board[0][1]->pos[1] = 1;
    this->board[0][1]->InputOffsets(this->board[0][1]);
    this->board[0][6] = new Knight(true);
    this->board[0][6]->pos[0] = 0;
    this->board[0][6]->pos[1] = 6;
    this->board[0][6]->InputOffsets(this->board[0][6]);
    this->board[7][1] = new Knight(false);
    this->board[7][1]->pos[0] = 7;
    this->board[7][1]->pos[1] = 1;
    this->board[7][1]->InputOffsets(this->board[7][1]);
    this->board[7][6] = new Knight(false);
    this->board[7][6]->pos[0] = 7;
    this->board[7][6]->pos[1] = 6;
    this->board[7][6]->InputOffsets(this->board[7][6]);

    this->board[0][2] = new Bishop(true);
    this->board[0][2]->pos[0] = 0;
    this->board[0][2]->pos[1] = 2;
    this->board[0][2]->InputOffsets(this->board[0][2]);
    this->board[0][5] = new Bishop(true);
    this->board[0][5]->pos[0] = 0;
    this->board[0][5]->pos[1] = 5;
    this->board[0][5]->InputOffsets(this->board[0][5]);
    this->board[7][2] = new Bishop(false);
    this->board[7][2]->pos[0] = 7;
    this->board[7][2]->pos[1] = 2;
    this->board[7][2]->InputOffsets(this->board[7][2]);
    this->board[7][5] = new Bishop(false);
    this->board[7][5]->pos[0] = 7;
    this->board[7][5]->pos[1] = 5;
    this->board[7][5]->InputOffsets(this->board[7][5]);

    this->board[0][3] = new King(true);
    this->board[0][3]->pos[0] = 0;
    this->board[0][3]->pos[1] = 3;
    this->board[0][3]->InputOffsets(this->board[0][3]);
    this->board[7][3] = new King(false);
    this->board[7][3]->pos[0] = 7;
    this->board[7][3]->pos[1] = 3;
    this->board[7][3]->InputOffsets(this->board[7][3]);

    this->board[0][4] = new Queen(true);
    this->board[0][4]->pos[0] = 0;
    this->board[0][4]->pos[1] = 4;
    this->board[0][4]->InputOffsets(this->board[0][4]);
    this->board[7][4] = new Queen(false);
    this->board[7][4]->pos[0] = 7;
    this->board[7][4]->pos[1] = 4;
    this->board[7][4]->InputOffsets(this->board[7][4]);
}

void ChessBoard::CalculatePossibleMoves(ChessPiece *piece, ChessBoard& board){
    vector<vector<int>> PossibleMoves = {};
    vector<vector<int>> withinRange;
    vector<vector<int>> AllOffSets = piece->AllOffSets;
    int x = piece->pos[0];
    int y = piece->pos[1];
    for (int i = 0; i < AllOffSets.size(); i++){
        int x1 = x + AllOffSets[i][0];
        int y1 = y + AllOffSets[i][1];
        if (x1 >= 0 && x1 <= 7 && y1 >= 0 && y1 <= 7){
            withinRange.push_back({x1, y1});
        }
    }

    if(piece->getSymbol() == 'P' || piece->getSymbol() == 'p'){
        for(int i = 0; i < withinRange.size(); i++){
            if(withinRange[i][1]!=y){
                if(board.board[withinRange[i][0]][withinRange[i][1]] != NULL){
                    if(board.board[withinRange[i][0]][withinRange[i][1]]->getIsWhite() != piece->getIsWhite()){
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                    }
                }
            }
            if(withinRange[i][1]==y){
                bool isInOriginalPosition = false;
                if(piece->getIsWhite()){
                    if(piece->pos[0] == 1){
                        isInOriginalPosition = true;
                    }
                }
                else{
                    if(piece->pos[0] == 6){
                        isInOriginalPosition = true;
                    }
                }
                if(isInOriginalPosition){
                    if(board.board[withinRange[i][0]][withinRange[i][1]] == NULL){
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                    }
                }
                else{
                    if(board.board[withinRange[i][0]][withinRange[i][1]] == NULL && withinRange[i][0] == x + 1){
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                    }
                }
            }
        }
    }
    else if(piece->getSymbol() == 'R' || piece->getSymbol() == 'r'){
        for(int i = 0; i < withinRange.size(); i++){
            if(withinRange[i][0] == x){
                if(board.board[withinRange[i][0]][withinRange[i][1]] == NULL){
                    if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                }
                else if(board.board[withinRange[i][0]][withinRange[i][1]]->getIsWhite() != piece->getIsWhite()){
                    if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                }
            }
            else if(withinRange[i][1] == y){
                if(board.board[withinRange[i][0]][withinRange[i][1]] == NULL){
                    if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                }
                else if(board.board[withinRange[i][0]][withinRange[i][1]]->getIsWhite() != piece->getIsWhite()){
                    if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                }
            }
        }
    }
    else if(piece->getSymbol() == 'N' || piece->getSymbol() == 'n'){
        for(int i = 0; i < withinRange.size(); i++){
            if(board.board[withinRange[i][0]][withinRange[i][1]] == NULL){
                PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
            }
            else if(board.board[withinRange[i][0]][withinRange[i][1]]->getIsWhite() != piece->getIsWhite()){
                PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
            }
        }
    }
    else if(piece->getSymbol() == 'B' || piece->getSymbol() == 'b'){
        for(int i = 0; i < withinRange.size(); i++){
            if(board.board[withinRange[i][0]][withinRange[i][1]] == NULL){
                if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                    PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
            }
            else if(board.board[withinRange[i][0]][withinRange[i][1]]->getIsWhite() != piece->getIsWhite()){
                if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                    PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
            }
        }
    }
    else if(piece->getSymbol() == 'Q' || piece->getSymbol() == 'q'){
        for(int i = 0; i < withinRange.size(); i++){
            if(withinRange[i][0] == x){
                if(board.board[withinRange[i][0]][withinRange[i][1]] == NULL){
                    if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                }
                else if(board.board[withinRange[i][0]][withinRange[i][1]]->getIsWhite() != piece->getIsWhite()){
                    if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                }
            }
            else if(withinRange[i][1] == y){
                if(board.board[withinRange[i][0]][withinRange[i][1]] == NULL){
                    if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                }
                else if(board.board[withinRange[i][0]][withinRange[i][1]]->getIsWhite() != piece->getIsWhite()){
                    if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                }
            }
            else if(abs(withinRange[i][0] - x) == abs(withinRange[i][1] - y)){
                if(board.board[withinRange[i][0]][withinRange[i][1]] == NULL){
                    if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                }
                else if(board.board[withinRange[i][0]][withinRange[i][1]]->getIsWhite() != piece->getIsWhite()){
                    if(!IsThereAPieceBetween(piece, withinRange[i][0], withinRange[i][1]))
                        PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
                }
            }
        }  
    }
    else if(piece->getSymbol() == 'K' || piece->getSymbol() == 'k'){
        for(int i = 0; i < withinRange.size(); i++){
            if(board.board[withinRange[i][0]][withinRange[i][1]] == NULL){
                PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
            }
            else if(board.board[withinRange[i][0]][withinRange[i][1]]->getIsWhite() != piece->getIsWhite()){
                PossibleMoves.push_back({withinRange[i][0], withinRange[i][1]});
            }
        }
    }
    piece->PossibleMoves = PossibleMoves;
}

bool ChessBoard::IsThereAPieceBetween(ChessPiece *piece, int x, int y) {
    int x1 = piece->pos[0];
    int y1 = piece->pos[1];
    int x2 = x;
    int y2 = y;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = max(abs(dx), abs(dy));
    int xinc = (dx != 0) ? dx / abs(dx) : 0;  
    int yinc = (dy != 0) ? dy / abs(dy) : 0;
    for (int i = 1; i < steps; i++) {
        if (this->board[x1 + i * xinc][y1 + i * yinc] != NULL) {
            return true;
        }
    }
    return false;
}

void ChessBoard::MovePieceForPlayer(ChessPiece *piece, int x, int y){
    int x1 = piece->pos[0];
    int y1 = piece->pos[1];
    this->board[x][y] = piece;
    this->board[x1][y1] = NULL;
    this->board[x][y]->pos[0] = x;
    this->board[x][y]->pos[1] = y;
}

void ChessBoard::MovePieceForAI(ChessPiece *piece, int x, int y){
    int x1 = piece->pos[0];
    int y1 = piece->pos[1];
    this->board[x][y] = piece;
    this->board[x1][y1] = NULL;
    this->board[x][y]->pos[0] = x;
    this->board[x][y]->pos[1] = y;
}

void ChessBoard::UpgradePawn(ChessPiece *piece, char symbol) {
    bool isWhite = piece->getIsWhite();
    bool isPawnAtEnd = (isWhite && piece->pos[0] == 7 && piece->getSymbol() == 'P') || (!isWhite && piece->pos[0] == 0 && piece->getSymbol() == 'p');

    if (isPawnAtEnd) {
        delete this->board[piece->pos[0]][piece->pos[1]];

        switch (symbol) {
            case 'R':
            case 'r':
                this->board[piece->pos[0]][piece->pos[1]] = new Rook(isWhite);
                break;
            case 'N':
            case 'n':
                this->board[piece->pos[0]][piece->pos[1]] = new Knight(isWhite);
                break;
            case 'B':
            case 'b':
                this->board[piece->pos[0]][piece->pos[1]] = new Bishop(isWhite);
                break;
            case 'Q':
            case 'q':
                this->board[piece->pos[0]][piece->pos[1]] = new Queen(isWhite);
                break;
            default:
                break;
        }

        this->board[piece->pos[0]][piece->pos[1]]->pos[0] = piece->pos[0];
        this->board[piece->pos[0]][piece->pos[1]]->pos[1] = piece->pos[1];
        this->board[piece->pos[0]][piece->pos[1]]->InputOffsets(this->board[piece->pos[0]][piece->pos[1]]);
    }
}

bool ChessBoard::IsPiecePinned(ChessPiece *piece, ChessBoard& board){
    for(int i = 0; i < board.board.size(); i++){
        for(int j = 0; j < board.board[i].size(); j++){
            if(board.board[i][j] != NULL){
                if(board.board[i][j]->getIsWhite() != piece->getIsWhite()){
                    board.CalculatePossibleMoves(board.board[i][j], board);
                    for(int k = 0; k < board.board[i][j]->PossibleMoves.size(); k++){
                        if(board.board[i][j]->PossibleMoves[k][0] == piece->pos[0] && board.board[i][j]->PossibleMoves[k][1] == piece->pos[1]){
                            piece->setIsPinned(true);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool ChessBoard::IsPiecePinning(ChessPiece *piece, ChessBoard& board){
    for(int i = 0; i < piece->PossibleMoves.size(); i++){
        if(board.board[piece->PossibleMoves[i][0]][piece->PossibleMoves[i][1]] != NULL){
            if(board.board[piece->PossibleMoves[i][0]][piece->PossibleMoves[i][1]]->getIsWhite() != piece->getIsWhite()){
                if(board.board[piece->PossibleMoves[i][0]][piece->PossibleMoves[i][1]]->getSymbol() == 'K' || board.board[piece->PossibleMoves[i][0]][piece->PossibleMoves[i][1]]->getSymbol() == 'k'){
                    piece->setIsPinningKing(true);
                }
                piece->setIsPinning(true);
                return true;
            }
        }
    }
    return false;
}

void ChessBoard::DeepCopy(ChessBoard& board, ChessBoard& newBoard){
    for(int i = 0; i < newBoard.board.size(); i++){
        for(int j = 0; j < newBoard.board[i].size(); j++){
            newBoard.board[i][j] = NULL;
        }
    }
    for(int i = 0; i < board.board.size(); i++){
        for(int j = 0; j < board.board[i].size(); j++){
            if(board.board[i][j] != NULL){
                if(board.board[i][j]->getSymbol() == 'P'){
                    newBoard.board[i][j] = new Pawn(true);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else if(board.board[i][j]->getSymbol() == 'p'){
                    newBoard.board[i][j] = new Pawn(false);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else if(board.board[i][j]->getSymbol() == 'R'){
                    newBoard.board[i][j] = new Rook(true);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else if(board.board[i][j]->getSymbol() == 'r'){
                    newBoard.board[i][j] = new Rook(false);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else if(board.board[i][j]->getSymbol() == 'N'){
                    newBoard.board[i][j] = new Knight(true);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else if(board.board[i][j]->getSymbol() == 'n'){
                    newBoard.board[i][j] = new Knight(false);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else if(board.board[i][j]->getSymbol() == 'B'){
                    newBoard.board[i][j] = new Bishop(true);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else if(board.board[i][j]->getSymbol() == 'b'){
                    newBoard.board[i][j] = new Bishop(false);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else if(board.board[i][j]->getSymbol() == 'Q'){
                    newBoard.board[i][j] = new Queen(true);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else if(board.board[i][j]->getSymbol() == 'q'){
                    newBoard.board[i][j] = new Queen(false);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else if(board.board[i][j]->getSymbol() == 'K'){
                    newBoard.board[i][j] = new King(true);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
                else{
                    newBoard.board[i][j] = new King(false);
                    newBoard.board[i][j]->pos[0] = i;
                    newBoard.board[i][j]->pos[1] = j;
                    newBoard.board[i][j]->InputOffsets(newBoard.board[i][j]);
                }
            }
        }
    }
}

string ChessBoard::TurnBoardToString(){
    string boardString = "";
    for(int i = 0; i < this->board.size(); i++){
        for(int j = 0; j < this->board[i].size(); j++){
            if(this->board[i][j] == NULL){
                boardString += "0";
            }
            else{
                boardString += this->board[i][j]->getSymbol();
            }
        }
    }
    return boardString;
}