#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setMinimumSize(600, 400);

    this->resize(800, 600);

    connect(ui->btnSingleplayer, &QPushButton::clicked, this, &MainWindow::on_btnSingleplayer_clicked);
    connect(ui->btnMultiplayer, &QPushButton::clicked, this, &MainWindow::on_btnMultiplayer_clicked);
    connect(ui->btnSettings, &QPushButton::clicked, this, &MainWindow::on_btnSettings_clicked);
    connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::on_btnExit_clicked);

    ui->comboDifficulty->addItem("Легкий", static_cast<int>(Difficulty::Easy));
    ui->comboDifficulty->addItem("Средний", static_cast<int>(Difficulty::Medium));
    ui->comboDifficulty->addItem("Сложный", static_cast<int>(Difficulty::Hard));

    setupGridConnections();

    connect(ui->btnBackGame, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);
    connect(ui->btnBackSettings, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);
    connect(ui->comboDifficulty, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onDifficultyChanged);

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupGridConnections() {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            QString btnName = QString("button_%1_%2").arg(r).arg(c);
            QPushButton *btn = this->findChild<QPushButton*>(btnName);

            if (btn) {
                btn->setProperty("row", r);
                btn->setProperty("col", c);
                connect(btn, &QPushButton::clicked, this, &MainWindow::onGridButtonClicked);
            }
        }
    }
}

void MainWindow::on_btnSingleplayer_clicked() {
    game.reset();
    updateUI();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btnMultiplayer_clicked() {
    QMessageBox::information(this, "Онлайн", "Сетевой режим в разработке!");
}

void MainWindow::on_btnSettings_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onBackToMenuClicked() {
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onDifficultyChanged(int index) {
    game.setDifficulty(static_cast<Difficulty>(index));
}

void MainWindow::onGridButtonClicked() {
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

void MainWindow::updateUI() {

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

        onBackToMenuClicked();
    }
}

void MainWindow::on_btnExit_clicked() {
    this->close();
}