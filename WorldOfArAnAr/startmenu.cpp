#include "startmenu.h"

namespace {
    const QString TITLE_LABEL = "QLabel { "
        "color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #9d50bb, stop:1 #6e48aa); "
        "font-size: 52px; "
        "font-weight: bold; "
        "font-family: 'Papyrus', 'Fantasy', 'Cursive'; "
        "letter-spacing: 2px; "
        "padding: 20px; "
              "}";

    const QString START_GAME_BUTTON_STYLE = "QPushButton { "
                                            "background-color: rgba(0, 100, 0, 150); "
                                            "color: white; "
                                            "border: 2px solid #005000; "
                                            "border-radius: 5px; "
                                            "padding: 15px; "
                                            "font-weight: bold; "
                                            "min-width: 200px; "
                                            "font-size: 16px; "
                                            "}"
                                            "QPushButton:hover { "
                                            "background-color: rgba(0, 130, 0, 200); "
                                            "}";
    const QString END_GAME_BUTTON_STYLE = "QPushButton { "
                                          "background-color: rgba(100, 0, 0, 150); "
                                          "color: white; "
                                          "border: 2px solid #500000; "
                                          "border-radius: 5px; "
                                          "padding: 15px; "
                                          "font-weight: bold; "
                                          "min-width: 200px; "
                                          "font-size: 16px; "
                                          "}"
                                          "QPushButton:hover { "
                                          "background-color: rgba(130, 0, 0, 200); "
                                          "}";
}
StartMenu::StartMenu(QWidget *parent) : QWidget(parent)
{
    QLabel* background = new QLabel(this);
    background->setPixmap(QPixmap(":/pictures/backgrounds/main_menu.jpg"));
    background->setScaledContents(true);
    background->setGeometry(0, 0, 800, 600);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(50, 50, 50, 50);
    layout->setSpacing(30);

    QLabel *titleLabel = new QLabel("World of ArAnAr", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(TITLE_LABEL);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(15);
    shadow->setOffset(4, 4);
    shadow->setColor(QColor(0, 0, 0, 180));
    titleLabel->setGraphicsEffect(shadow);

    startButton = new QPushButton("Start Game", this);
    exitButton = new QPushButton("Exit", this);

    startButton->setStyleSheet(START_GAME_BUTTON_STYLE);

    exitButton->setStyleSheet(END_GAME_BUTTON_STYLE);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addStretch();
    layout->addWidget(startButton);
    layout->addWidget(exitButton);

    connect(startButton, &QPushButton::clicked, this, &StartMenu::startGameRequested);
    connect(exitButton, &QPushButton::clicked, this, &StartMenu::exitRequested);

    setLayout(layout);
}
