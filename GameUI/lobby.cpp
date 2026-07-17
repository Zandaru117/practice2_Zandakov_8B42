#include "lobby.h"
#include "ui_lobby.h"

Lobby::Lobby(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Lobby)
{
    ui->setupUi(this);

    connect(ui->btnBackLobby, &QPushButton::clicked, this, &Lobby::onBackClicked);
}

Lobby::~Lobby() {
    delete ui;
}

void Lobby::onBackClicked() {
    emit backToMenuRequested();
}