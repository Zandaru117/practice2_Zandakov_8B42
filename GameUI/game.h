#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "../GameCore/game_logic.h"

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();

    void startNewGame();
    void setDifficulty(Difficulty diff);

signals:
    void backToMenuRequested();

private slots:
    void onGridButtonClicked();
    void onBackClicked();

private:
    Ui::Game *ui;
    TicTacToe game;

    void setupGridConnections();
    void updateUI();
};

#endif // GAME_H