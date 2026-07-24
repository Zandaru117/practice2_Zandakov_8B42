#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainmenu.h"
#include "game.h"
#include "settings.h"
#include "lobby.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setMinimumSize(600, 400);
    this->resize(800, 600);

    menuWidget     = new MainMenu(this);
    gameWidget     = new Game(this);
    settingsWidget = new Settings(this);
    lobbyWidget    = new Lobby(this);

    ui->stackedWidget->addWidget(menuWidget);     // 0
    ui->stackedWidget->addWidget(gameWidget);     // 1
    ui->stackedWidget->addWidget(settingsWidget); // 2
    ui->stackedWidget->addWidget(lobbyWidget);    // 3

    ui->stackedWidget->setCurrentIndex(0);

    connect(menuWidget, &MainMenu::singleplayerRequested, this, [this]() {
        gameWidget->startNewGame();
        ui->stackedWidget->setCurrentWidget(gameWidget);
    });

    connect(menuWidget, &MainMenu::multiplayerRequested, this, [this]() {
        ui->stackedWidget->setCurrentWidget(lobbyWidget);
    });

    connect(menuWidget, &MainMenu::settingsRequested, this, [this]() {
        ui->stackedWidget->setCurrentWidget(settingsWidget);
    });

    connect(menuWidget, &MainMenu::exitRequested, this, &MainWindow::close);

    connect(gameWidget, &Game::backToMenuRequested, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(settingsWidget, &Settings::backToMenuRequested, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(lobbyWidget, &Lobby::backToMenuRequested, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(lobbyWidget, &Lobby::networkGameStarted, this, [this](QTcpSocket* socket, bool isHost) {
        gameWidget->startNetworkGame(socket, isHost);
        ui->stackedWidget->setCurrentWidget(gameWidget);
    });

    connect(lobbyWidget, &Lobby::opponentDisconnected, gameWidget, &Game::onDisconnected);

    connect(settingsWidget, &Settings::difficultyChanged, gameWidget, &Game::setDifficulty);
}

MainWindow::~MainWindow() {
    delete ui;
}