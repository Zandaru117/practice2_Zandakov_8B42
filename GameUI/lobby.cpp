#include "lobby.h"
#include "ui_lobby.h"
#include <QNetworkInterface>
#include <QMessageBox>

Lobby::Lobby(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Lobby),
    tcpServer(nullptr),
    tcpSocket(nullptr),
    activeSocket(nullptr)
{
    ui->setupUi(this);

    QString localIP = getLocalIP();
    ui->lblHostIP->setText("Ваш IP: " + localIP);

    connect(ui->btnBackLobby, &QPushButton::clicked, this, &Lobby::onBackClicked);
    connect(ui->btnCreateRoom, &QPushButton::clicked, this, &Lobby::onCreateRoomClicked);
    connect(ui->btnConnect, &QPushButton::clicked, this, &Lobby::onConnectClicked);
}

Lobby::~Lobby() {
    if (tcpServer) tcpServer->close();
    if (tcpSocket) tcpSocket->close();
    if (activeSocket) activeSocket->close();
    qDeleteAll(waitingQueue);
    delete ui;
}

QString Lobby::getLocalIP() {
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address != localhost && address.protocol() == QAbstractSocket::IPv4Protocol) {
            return address.toString();
        }
    }
    return "127.0.0.1";
}

void Lobby::onCreateRoomClicked() {
    if (!tcpServer) {
        tcpServer = new QTcpServer(this);
        connect(tcpServer, &QTcpServer::newConnection, this, &Lobby::onNewConnection);
    }

    if (!tcpServer->listen(QHostAddress::Any, 12345)) {
        ui->lblHostStatus->setText("Ошибка запуска сервера");
        return;
    }

    ui->lblHostStatus->setText("Ожидание соперника (порт 12345)...");
    ui->btnCreateRoom->setEnabled(false);
    ui->btnConnect->setEnabled(false);
}

void Lobby::onNewConnection() {
    while (tcpServer->hasPendingConnections()) {
        QTcpSocket* newClient = tcpServer->nextPendingConnection();
        waitingQueue.append(newClient);

        connect(newClient, &QTcpSocket::disconnected, this, [this, newClient]() {

            if (activeSocket == newClient) {
                onActiveMatchFinished();
            } else {

                waitingQueue.removeAll(newClient);
                newClient->deleteLater();
            }
        });

        if (activeSocket != nullptr || waitingQueue.size() > 1) {
            QString waitMsg = QString("WAIT %1\n").arg(waitingQueue.size());
            newClient->write(waitMsg.toUtf8());
            newClient->flush();
        }
    }

    if (activeSocket == nullptr && !waitingQueue.isEmpty()) {
        startNextMatch();
    }
}

void Lobby::startNextMatch() {
    if (waitingQueue.isEmpty()) {
        activeSocket = nullptr;
        ui->lblHostStatus->setText("Очередь пуста. Ожидание игроков...");
        return;
    }

    activeSocket = waitingQueue.takeFirst();
    ui->lblHostStatus->setText("Соперник подключился из очереди!");

    activeSocket->write("START\n");
    activeSocket->flush();

    emit networkGameStarted(activeSocket, true);
}

void Lobby::onActiveMatchFinished() {
    activeSocket = nullptr;

    emit opponentDisconnected();

    startNextMatch();
}

void Lobby::onConnectClicked() {
    QString targetIP = ui->txtServerIP->text().trimmed();
    if (targetIP.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите IP-адрес соперника!");
        return;
    }

    if (!tcpSocket) {
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket, &QTcpSocket::connected, this, &Lobby::onConnected);
        connect(tcpSocket, &QTcpSocket::readyRead, this, &Lobby::onClientReadyRead);
        connect(tcpSocket, &QAbstractSocket::errorOccurred, this, [this](QAbstractSocket::SocketError) {
            ui->lblClientStatus->setText("Ошибка подключения");
            QMessageBox::critical(this, "Ошибка", "Не удалось подключиться: " + tcpSocket->errorString());
        });
    }

    ui->lblClientStatus->setText("Подключение к " + targetIP + "...");
    tcpSocket->connectToHost(targetIP, 12345);
}

void Lobby::onConnected() {
    ui->lblClientStatus->setText("Успешно подключено! Ожидание очереди...");
}

void Lobby::onErrorOccurred(QAbstractSocket::SocketError) {
    ui->lblClientStatus->setText("Ошибка подключения");
    QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к хосту: " + tcpSocket->errorString());
}

void Lobby::onBackClicked() {
    if (tcpServer && tcpServer->isListening()) {
        tcpServer->close();
    }
    ui->btnCreateRoom->setEnabled(true);
    ui->btnConnect->setEnabled(true);
    ui->lblHostStatus->setText("Ожидание подключения...");
    ui->lblClientStatus->setText("Подключение");
    emit backToMenuRequested();
}

void Lobby::onClientReadyRead() {
    if (!tcpSocket) return;

    while (tcpSocket->canReadLine()) {
        QByteArray line = tcpSocket->readLine().trimmed();

        if (line.startsWith("WAIT")) {
            QList<QByteArray> tokens = line.split(' ');
            QString pos = (tokens.size() > 1) ? tokens[1] : "1";
            ui->lblClientStatus->setText(QString("Комната занята. Вы в очереди (Позиция: %1)...").arg(pos));
        }

        if (line == "START") {
            ui->lblClientStatus->setText("Ваша очередь подошла! Запуск матча...");

            disconnect(tcpSocket, &QTcpSocket::readyRead, this, &Lobby::onClientReadyRead);

            emit networkGameStarted(tcpSocket, false);
        }
    }
}