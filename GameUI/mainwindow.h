#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainMenu;
class Game;
class Settings;
class Lobby;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    MainMenu *menuWidget;
    Game     *gameWidget;
    Settings *settingsWidget;
    Lobby    *lobbyWidget;
};

#endif // MAINWINDOW_H