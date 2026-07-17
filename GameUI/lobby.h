#ifndef LOBBY_H
#define LOBBY_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>

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

private slots:
    void onBackClicked();
    void onCreateRoomClicked();
    void onConnectClicked();

    void onNewConnection();
    void onConnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    Ui::Lobby *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    QString getLocalIP();
};

#endif // LOBBY_H
