#include "gamemenu.h"
#include "ui_gamemenu.h"

GameMenu::GameMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameMenu)
{
    ui->setupUi(this);
}

GameMenu::~GameMenu()
{
    delete ui;
}
