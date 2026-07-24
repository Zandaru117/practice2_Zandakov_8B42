#include "game.h"
#include "ui_game.h"
#include <QMessageBox>

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game),
    tcpSocket(nullptr),
    isNetworkMode(false),
    isMyTurn(true),
    mySign(Cell::X)
{
    ui->setupUi(this);
    setupGridConnections();
    connect(ui->btnBackGame, &QPushButton::clicked, this, &Game::onBackClicked);

    connect(ui->btnRestart, &QPushButton::clicked, this, &Game::onRestartClicked);
    ui->btnRestart->hide();

    connect(ui->btnSendChat, &QPushButton::clicked, this, &Game::onSendChatClicked);
    connect(ui->chatInput, &QLineEdit::returnPressed, this, &Game::onSendChatClicked);
}

Game::~Game() {
    delete ui;
}

void Game::startNewGame() {
    isNetworkMode = false;
    game.reset();
    updateUI();
}

void Game::startNetworkGame(QTcpSocket* socket, bool isHost) {
    tcpSocket = socket;
    isNetworkMode = true;
    isMyTurn = isHost;
    mySign = isHost ? Cell::X : Cell::O;

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Game::onReadyRead);

    ui->chatDisplay->clear();
    ui->chatDisplay->append("<i>Система: Соединение установлено. Вы можете общаться!</i>");

    game.reset();
    updateUI();
}

void Game::setDifficulty(Difficulty diff) {
    game.setDifficulty(diff);
}

void Game::setupGridConnections() {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            QString btnName = QString("button_%1_%2").arg(r).arg(c);
            QPushButton *btn = this->findChild<QPushButton*>(btnName);
            if (btn) {
                btn->setProperty("row", r);
                btn->setProperty("col", c);
                connect(btn, &QPushButton::clicked, this, &Game::onGridButtonClicked);
            }
        }
    }
}

void Game::onGridButtonClicked() {
    if (isNetworkMode && !isMyTurn) return;

    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    int row = clickedButton->property("row").toInt();
    int col = clickedButton->property("col").toInt();

    if (game.makeMove(row, col)) {
        updateUI();

        GameState state = game.getState();
        if (state != GameState::Progress) {
            showFinalMessage(state);
        }

        if (isNetworkMode) {
            QString moveStr = QString("%1 %2\n").arg(row).arg(col);
            tcpSocket->write(moveStr.toUtf8());
            isMyTurn = false;
            updateUI();
        } else {
            if (game.getState() == GameState::Progress) {
                game.makeBotMove();
                updateUI();

                if (game.getState() != GameState::Progress) {
                    showFinalMessage(game.getState());
                }
            }
        }
    }
}

void Game::onReadyRead() {
    if (!tcpSocket) return;

    while (tcpSocket->canReadLine()) {
        QByteArray line = tcpSocket->readLine().trimmed();

        if (line == "RESTART") {
            game.reset();
            isMyTurn = (mySign == Cell::X);
            ui->btnRestart->hide();
            updateUI();
            continue;
        }

        if (line.startsWith("CHAT")) {
            QString message = QString::fromUtf8(line.mid(4));
            ui->chatDisplay->append(QString("<b>Соперник:</b> %1").arg(message));
            continue;
        }

        QList<QByteArray> tokens = line.split(' ');
        if (tokens.size() == 2) {
            int row = tokens[0].toInt();
            int col = tokens[1].toInt();

            if (game.makeMove(row, col)) {
                isMyTurn = true;
                updateUI();

                if (game.getState() != GameState::Progress) {
                    showFinalMessage(game.getState());
                }
            }
        }
    }
}

void Game::onDisconnected() {
    tcpSocket = nullptr;

    if (isNetworkMode && mySign == Cell::X) {
        ui->statusLabel->setText("Соперник вышел. Ожидание игрока из очереди...");
        ui->chatDisplay->append("<i>Система: Соперник покинул игру. Подключаем следующего игрока...</i>");

        game.reset();
        updateUI();
    } else {
        QMessageBox::information(this, "Игра окончена", "Сессия закрыта сервером.");
        onBackClicked();
    }
}

void Game::updateUI() {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            QString btnName = QString("button_%1_%2").arg(r).arg(c);
            QPushButton *btn = this->findChild<QPushButton*>(btnName);
            if (btn) {
                Cell cell = game.getCell(r, c);
                if (cell == Cell::X) btn->setText("X");
                else if (cell == Cell::O) btn->setText("O");
                else btn->setText("");
            }
        }
    }

    GameState state = game.getState();
    if (state == GameState::Progress) {
        ui->btnRestart->hide();
        if (isNetworkMode) {
            ui->statusLabel->setText(isMyTurn ? "Ваш ход!" : "Ход соперника...");
        } else {
            ui->statusLabel->setText(game.getCurrentPlayer() == Cell::X ? "Ваш ход (X)" : "Ход компьютера (O)");
        }
    } else {

        ui->btnRestart->show();

        if (state == GameState::WinX) {
            ui->statusLabel->setText(isNetworkMode ? (mySign == Cell::X ? "Вы победили!" : "Победил соперник (X)!") : "Вы выиграли!");
        } else if (state == GameState::WinO) {
            ui->statusLabel->setText(isNetworkMode ? (mySign == Cell::O ? "Вы победили!" : "Победил соперник (O)!") : "Компьютер выиграл!");
        } else if (state == GameState::Draw) {
            ui->statusLabel->setText("Ничья!");
        }

    }
}

void Game::onBackClicked() {
    if (tcpSocket) {
        tcpSocket->disconnectFromHost();
        tcpSocket = nullptr;
    }
    emit backToMenuRequested();
}

void Game::onRestartClicked() {
    game.reset();

    if (isNetworkMode && tcpSocket) {
        tcpSocket->write("RESTART\n");
        isMyTurn = (mySign == Cell::X);
    } else {
        isMyTurn = true;
    }

    ui->btnRestart->hide();
    updateUI();
}


void Game::showFinalMessage(GameState state) {
    if (state == GameState::WinX) {
        QMessageBox::information(this, "Финал", isNetworkMode ? (mySign == Cell::X ? "Вы победили!" : "Победил соперник (X)!") : "Вы выиграли!");
    } else if (state == GameState::WinO) {
        QMessageBox::critical(this, "Финал", isNetworkMode ? (mySign == Cell::O ? "Вы победили!" : "Победил соперник (O)!") : "Компьютер выиграл!");
    } else if (state == GameState::Draw) {
        QMessageBox::information(this, "Финал", "Ничья!");
    }
}


void Game::onSendChatClicked() {
    QString text = ui->chatInput->text().trimmed();
    if (text.isEmpty()) return;

    if (isNetworkMode && tcpSocket) {
        QString packet = QString("CHAT%1\n").arg(text);
        tcpSocket->write(packet.toUtf8());

        ui->chatDisplay->append(QString("<b>Вы:</b> %1").arg(text));
    } else {
        ui->chatDisplay->append("<i>Система: Чат доступен только в сетевой игре</i>");
    }

    ui->chatInput->clear();
}