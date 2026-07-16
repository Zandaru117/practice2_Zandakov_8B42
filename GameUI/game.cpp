#include "game.h"
#include "ui_game.h"
#include <QMessageBox>

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    setupGridConnections();
    connect(ui->btnBackGame, &QPushButton::clicked, this, &Game::onBackClicked);
}

Game::~Game() {
    delete ui;
}

void Game::startNewGame() {
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
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    int row = clickedButton->property("row").toInt();
    int col = clickedButton->property("col").toInt();

    if (game.makeMove(row, col)) {
        updateUI();
        if (game.getState() == GameState::Progress) {
            game.makeBotMove();
            updateUI();
        }
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
        if (game.getCurrentPlayer() == Cell::X) {
            ui->statusLabel->setText("Ваш ход (X)");
        } else {
            ui->statusLabel->setText("Ход компьютера (O)");
        }
    } else {
        if (state == GameState::WinX) QMessageBox::information(this, "Финал", "Вы выиграли!");
        else if (state == GameState::WinO) QMessageBox::critical(this, "Финал", "Компьютер выиграл!");
        else if (state == GameState::Draw) QMessageBox::information(this, "Финал", "Ничья!");

        emit backToMenuRequested();
    }
}

void Game::onBackClicked() {
    emit backToMenuRequested();
}