#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

namespace Ui {
class Lobby;
}

class Lobby : public QWidget
{
    Q_OBJECT

public:
    explicit Lobby(QWidget *parent = nullptr);
    ~Lobby();

signals:
    void backToMenuRequested();
    void networkGameStarted(QTcpSocket* socket, bool isHost);
    void opponentDisconnected();

private slots:
    void onBackClicked();
    void onCreateRoomClicked();
    void onConnectClicked();

    void onNewConnection();
    void onConnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onClientReadyRead();
    void onActiveMatchFinished();

private:
    Ui::Lobby *ui;
    QTcpServer *tcpServer;

    QTcpSocket *tcpSocket;     
    QTcpSocket *activeSocket;
    QList<QTcpSocket*> waitingQueue;

    QString getLocalIP();
    void startNextMatch();
};

#endif // LOBBY_H
