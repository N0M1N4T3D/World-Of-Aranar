#include "singletonlogger.h"

SingletonLogger* SingletonLogger::instance = nullptr;

SingletonLogger::SingletonLogger(QObject *parent) : QObject(parent)
{
    logWindow = new QDialog();
    logWindow->setWindowTitle("Game Log");
    logWindow->resize(600, 400);

    QVBoxLayout* layout = new QVBoxLayout(logWindow);

    logTextEdit = new QTextEdit(logWindow);
    logTextEdit->setReadOnly(true);
    layout->addWidget(logTextEdit);

    QPushButton* closeButton = new QPushButton("Close", logWindow);
    connect(closeButton, &QPushButton::clicked, logWindow, &QDialog::hide);
    layout->addWidget(closeButton);

    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - logWindow->width()) / 2;
        int y = (screenGeometry.height() - logWindow->height()) / 2;
        logWindow->move(x, y);
    }
}

SingletonLogger* SingletonLogger::getInstance()
{
    if (!instance) {
        instance = new SingletonLogger();
    }
    return instance;
}

void SingletonLogger::log(const QString& message)
{
    logTextEdit->append(message);
}

void SingletonLogger::showLogWindow()
{
    logWindow->show();
    logWindow->raise();
    logWindow->activateWindow();
}
