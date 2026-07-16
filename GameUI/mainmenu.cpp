#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    connect(ui->btnSingleplayer, &QPushButton::clicked, this, &MainMenu::singleplayerRequested);
    connect(ui->btnMultiplayer, &QPushButton::clicked, this, &MainMenu::multiplayerRequested);
    connect(ui->btnSettings, &QPushButton::clicked, this, &MainMenu::settingsRequested);
    connect(ui->btnExit, &QPushButton::clicked, this, &MainMenu::exitRequested);
}

MainMenu::~MainMenu() {
    delete ui;
}