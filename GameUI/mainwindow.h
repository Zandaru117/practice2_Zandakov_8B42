#pragma once

#include <QMainWindow>
#include "../GameCore/game_logic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSingleplayer_clicked();
    void on_btnMultiplayer_clicked();
    void on_btnSettings_clicked();
    void on_btnExit_clicked();

    void onGridButtonClicked();
    void onBackToMenuClicked();
    void onDifficultyChanged(int index);

private:
    Ui::MainWindow *ui;
    TicTacToe game;

    void setupGridConnections();
    void updateUI();
};