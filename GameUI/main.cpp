#include <QApplication>
#include <QPushButton>
#include "game_logic.h" // Подключаем логику

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Вызываем функцию из нашей библиотеки GameCore
    int number = getAnswer();

    QPushButton button(QString("Ответ из GameCore: %1").arg(number));
    button.resize(300, 100);
    button.show();

    return a.exec();
}