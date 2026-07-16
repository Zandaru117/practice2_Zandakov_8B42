#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    ui->comboDifficulty->addItem("Легкий", static_cast<int>(Difficulty::Easy));
    ui->comboDifficulty->addItem("Средний", static_cast<int>(Difficulty::Medium));
    ui->comboDifficulty->addItem("Сложный", static_cast<int>(Difficulty::Hard));

    connect(ui->btnBackSettings, &QPushButton::clicked, this, &Settings::backToMenuRequested);

    connect(ui->comboDifficulty, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        QVariant data = ui->comboDifficulty->itemData(index);
        emit difficultyChanged(static_cast<Difficulty>(data.toInt()));
    });
}

Settings::~Settings() {
    delete ui;
}