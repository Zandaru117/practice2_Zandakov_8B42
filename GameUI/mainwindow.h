#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QComboBox>
#include "../GameCore/game_logic.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void onMenuSingleplayerClicked();
    void onMenuMultiplayerClicked();
    void onMenuSettingsClicked();
    void onGridButtonClicked();
    void onBackToMenuClicked();
    void onDifficultyChanged(int index);

private:
    TicTacToe game;

    QStackedWidget *stackedWidget;

    QWidget *menuWidget;
    QWidget *gameWidget;
    QWidget *settingsWidget;

    QPushButton *gridButtons[3][3];
    QLabel *statusLabel;

    void createMenuScreen();
    void createGameScreen();
    void createSettingsScreen();
    void updateUI();
};