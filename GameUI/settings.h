#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "../GameCore/game_logic.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

signals:
    void backToMenuRequested();
    void difficultyChanged(Difficulty diff);

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H