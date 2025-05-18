#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

class StartMenu : public QWidget
{
    Q_OBJECT
public:
    explicit StartMenu(QWidget *parent = nullptr);

signals:
    void startGameRequested();
    void exitRequested();

private:
    QPushButton *startButton;
    QPushButton *exitButton;
};

#endif // STARTMENU_H
