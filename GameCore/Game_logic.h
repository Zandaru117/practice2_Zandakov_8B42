#pragma once

#include <vector>

enum class Cell { Empty, X, O };
enum class Difficulty { Easy, Medium, Hard };
enum class GameState { Progress, WinX, WinO, Draw };

class TicTacToe {
public:
    TicTacToe();

    bool makeMove(int row, int col);
    void makeBotMove();
    void reset();

    Cell getCell(int row, int col) const;
    Cell getCurrentPlayer() const;
    GameState getState() const;

    void setDifficulty(Difficulty diff);
    Difficulty getDifficulty() const;

private:
    Cell board[3][3];
    Cell currentPlayer;
    GameState state;    
    Difficulty difficulty; 

    void checkGameState();
    bool hasMovesLeft() const;

    void makeRandomMove();
    bool tryWinOrBlock(Cell targetCell);
    int evaluateBoard() const;
    int minimax(bool isMax);
};