#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    //Создаем экраны
    createMenuScreen();
    createGameScreen();
    createSettingsScreen();

    stackedWidget->addWidget(menuWidget);
    stackedWidget->addWidget(gameWidget);
    stackedWidget->addWidget(settingsWidget);

    //Стартуем с главного меню
    stackedWidget->setCurrentIndex(0);
    resize(400, 450);
}

void MainWindow::createMenuScreen() {
    menuWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(menuWidget);

    QLabel *title = new QLabel("Крестики-Нолики", menuWidget);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 20px;");
    layout->addWidget(title);

    QPushButton *btnSingle = new QPushButton("Игра против компьютера", menuWidget);
    QPushButton *btnOnline = new QPushButton("Онлайн игра (В разработке)", menuWidget);
    QPushButton *btnSettings = new QPushButton("Настройки", menuWidget);
    QPushButton *btnExit = new QPushButton("Выход", menuWidget);

    layout->addWidget(btnSingle);
    layout->addWidget(btnOnline);
    layout->addWidget(btnSettings);
    layout->addWidget(btnExit);

    connect(btnSingle, &QPushButton::clicked, this, &MainWindow::onMenuSingleplayerClicked);
    connect(btnOnline, &QPushButton::clicked, this, &MainWindow::onMenuMultiplayerClicked);
    connect(btnSettings, &QPushButton::clicked, this, &MainWindow::onMenuSettingsClicked);
    connect(btnExit, &QPushButton::clicked, this, &MainWindow::close);
}

void MainWindow::createGameScreen() {
    gameWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(gameWidget);

    statusLabel = new QLabel("Ход Крестиков (X)", gameWidget);
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    QGridLayout *gridLayout = new QGridLayout();
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            gridButtons[r][c] = new QPushButton(gameWidget);
            gridButtons[r][c]->setFixedSize(80, 80);
            gridButtons[r][c]->setStyleSheet("font-size: 24px; font-weight: bold;");

            gridButtons[r][c]->setProperty("row", r);
            gridButtons[r][c]->setProperty("col", c);

            gridLayout->addWidget(gridButtons[r][c], r, c);
            connect(gridButtons[r][c], &QPushButton::clicked, this, &MainWindow::onGridButtonClicked);
        }
    }
    mainLayout->addLayout(gridLayout);

    QPushButton *btnBack = new QPushButton("В главное меню", gameWidget);
    connect(btnBack, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);
    mainLayout->addWidget(btnBack);
}

void MainWindow::createSettingsScreen() {
    settingsWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(settingsWidget);

    QLabel *label = new QLabel("Сложность компьютера:", settingsWidget);
    QComboBox *combo = new QComboBox(settingsWidget);
    combo->addItem("Легкий", static_cast<int>(Difficulty::Easy));
    combo->addItem("Средний", static_cast<int>(Difficulty::Medium));
    combo->addItem("Сложный", static_cast<int>(Difficulty::Hard));

    layout->addWidget(label);
    layout->addWidget(combo);

    QPushButton *btnBack = new QPushButton("Назад", settingsWidget);
    layout->addWidget(btnBack);

    connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onDifficultyChanged);
    connect(btnBack, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);
}


void MainWindow::onMenuSingleplayerClicked() {
    game.reset();
    updateUI();
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::onMenuMultiplayerClicked() {
    QMessageBox::information(this, "Онлайн", "Сетевой режим появится в следующих обновлениях!");
}

void MainWindow::onMenuSettingsClicked() {
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::onBackToMenuClicked() {
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::onDifficultyChanged(int index) {
    Difficulty diff = static_cast<Difficulty>(index);
    game.setDifficulty(diff);
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
            Cell cell = game.getCell(r, c);
            if (cell == Cell::X) gridButtons[r][c]->setText("X");
            else if (cell == Cell::O) gridButtons[r][c]->setText("O");
            else gridButtons[r][c]->setText("");
        }
    }

    //Обработка состояний игры
    GameState state = game.getState();
    if (state == GameState::Progress) {
        if (game.getCurrentPlayer() == Cell::X) {
            statusLabel->setText("Ваш ход (X)");
        } else {
            statusLabel->setText("Ход компьютера (O)");
        }
    } else {
        if (state == GameState::WinX) {
            statusLabel->setText("Вы победили! (X)");
            QMessageBox::information(this, "Финал", "Поздравляем! Вы выиграли.");
        } else if (state == GameState::WinO) {
            statusLabel->setText("Победил компьютер! (O)");
            QMessageBox::critical(this, "Финал", "Компьютер выиграл. Попробуйте еще раз!");
        } else if (state == GameState::Draw) {
            statusLabel->setText("Ничья!");
            QMessageBox::information(this, "Финал", "Ничья!");
        }
        onBackToMenuClicked();
    }
}