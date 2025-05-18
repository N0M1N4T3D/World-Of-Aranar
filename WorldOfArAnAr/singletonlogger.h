#ifndef SINGLETONLOGGER_H
#define SINGLETONLOGGER_H

#include <QObject>
#include <QString>
#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QScreen>

class SingletonLogger : public QObject //Singleton pattern
{
    Q_OBJECT
private:
    static SingletonLogger* instance;
    QDialog* logWindow;
    QTextEdit* logTextEdit;

    explicit SingletonLogger(QObject *parent = nullptr);

public:
    static SingletonLogger* getInstance();
    void log(const QString& message);
    void showLogWindow();

    SingletonLogger(const SingletonLogger&) = delete;
    SingletonLogger& operator=(const SingletonLogger&) = delete;
};

#endif // SINGLETONLOGGER_H
