#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QTcpSocket>
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

    void startNetworkGame(QTcpSocket* socket, bool isHost);

public slots:
    void onDisconnected();

signals:
    void backToMenuRequested();
    void opponentDisconnected();

private slots:
    void onGridButtonClicked();
    void onBackClicked();
    void onReadyRead();
    void onRestartClicked();
    void onSendChatClicked();

private:
    Ui::Game *ui;
    TicTacToe game;

    QTcpSocket* tcpSocket;
    bool isNetworkMode;
    bool isMyTurn;
    Cell mySign;

    void setupGridConnections();
    void updateUI();
    void showFinalMessage(GameState state);
};

#endif // GAME_H