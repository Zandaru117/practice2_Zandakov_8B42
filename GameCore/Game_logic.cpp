#include "game_logic.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

TicTacToe::TicTacToe() {
    std::srand(std::time(nullptr));
    difficulty = Difficulty::Easy;
    reset();
}

void TicTacToe::reset() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) { 
            board[i][j] = Cell::Empty;
        }
    }
    currentPlayer = Cell::X; 
    state = GameState::Progress;
}

Cell TicTacToe::getCell(int row, int col) const {
    return board[row][col];
}

Cell TicTacToe::getCurrentPlayer() const {
    return currentPlayer;
}

GameState TicTacToe::getState() const {
    return state;
}

void TicTacToe::setDifficulty(Difficulty diff) {
    difficulty = diff;
}

Difficulty TicTacToe::getDifficulty() const {
    return difficulty; 
}

bool TicTacToe::makeMove(int row, int col) {
    if (row < 0 || row >= 3 || col < 0 || col >= 3) return false;
    if (board[row][col] != Cell::Empty || state != GameState::Progress) return false;

    board[row][col] = currentPlayer;
    checkGameState();

    if (state == GameState::Progress) {
        currentPlayer = (currentPlayer == Cell::X) ? Cell::O : Cell::X;
    }
    return true;
}

void TicTacToe::checkGameState() {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != Cell::Empty && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            state = (board[i][0] == Cell::X) ? GameState::WinX : GameState::WinO;
            return;
        }
        if (board[0][i] != Cell::Empty && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            state = (board[0][i] == Cell::X) ? GameState::WinX : GameState::WinO;
            return;
        }
    }

    if (board[0][0] != Cell::Empty && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        state = (board[0][0] == Cell::X) ? GameState::WinX : GameState::WinO;
        return;
    }
    if (board[0][2] != Cell::Empty && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        state = (board[0][2] == Cell::X) ? GameState::WinX : GameState::WinO;
        return;
    }

    if (!hasMovesLeft()) {
        state = GameState::Draw;
    }
}

bool TicTacToe::hasMovesLeft() const {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == Cell::Empty) return true;
    return false;
}

void TicTacToe::makeRandomMove() {
    int row, col;
    do {
        row = std::rand() % 3;
        col = std::rand() % 3;
    } while (board[row][col] != Cell::Empty);
    makeMove(row, col);
}

bool TicTacToe::tryWinOrBlock(Cell targetCell) {
    // Строки
    for (int r = 0; r < 3; ++r) {
        int count = 0, emptyCol = -1;
        for (int c = 0; c < 3; ++c) {
            if (board[r][c] == targetCell) count++;
            else if (board[r][c] == Cell::Empty) emptyCol = c;
        }
        if (count == 2 && emptyCol != -1) { makeMove(r, emptyCol); return true; }
    }

    for (int c = 0; c < 3; ++c) {
        int count = 0, emptyRow = -1;
        for (int r = 0; r < 3; ++r) {
            if (board[r][c] == targetCell) count++;
            else if (board[r][c] == Cell::Empty) emptyRow = r;
        }
        if (count == 2 && emptyRow != -1) { makeMove(emptyRow, c); return true; }
    }

    int countDiag1 = 0, emptyIdx1 = -1;
    for (int i = 0; i < 3; ++i) {
        if (board[i][i] == targetCell) countDiag1++;
        else if (board[i][i] == Cell::Empty) emptyIdx1 = i;
    }
    if (countDiag1 == 2 && emptyIdx1 != -1) { makeMove(emptyIdx1, emptyIdx1); return true; }

    int countDiag2 = 0, emptyRow2 = -1, emptyCol2 = -1;
    for (int i = 0; i < 3; ++i) {
        if (board[i][2 - i] == targetCell) countDiag2++;
        else if (board[i][2 - i] == Cell::Empty) { emptyRow2 = i; emptyCol2 = 2 - i; }
    }
    if (countDiag2 == 2 && emptyRow2 != -1) { makeMove(emptyRow2, emptyCol2); return true; }
    return false;
}

void TicTacToe::makeBotMove() {
    if (state != GameState::Progress) return;

    if (difficulty == Difficulty::Easy) {
        makeRandomMove();
        return;
    }

    if (difficulty == Difficulty::Medium) {
        if (tryWinOrBlock(currentPlayer)) return;
        Cell opponent = (currentPlayer == Cell::X) ? Cell::O : Cell::X;
        if (tryWinOrBlock(opponent)) return;
        if (board[1][1] == Cell::Empty) {
            makeMove(1, 1);
            return;
        }
        makeRandomMove();
        return;
    }

    if (difficulty == Difficulty::Hard) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == Cell::Empty) {
                    makeMove(i, j);
                    return;
                }
            }
        }
    }
}