#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QDebug>

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const QString& message) = 0;
};

class DeathLogger : public ILogger {
public:
    void log(const QString& message) override {
        if (message.contains("is defeated!")) {
            qDebug() << "[DeathLogger] " << message;
        }
    }
};

class AbilityLogger : public ILogger {
public:
    void log(const QString& message) override {
        if (message.contains("heals") || message.contains("shoots") || message.contains("cloned")) {
            qDebug() << "[AbilityLogger] " << message;
        }
    }
};

class LoggerProxy : public ILogger { //Proxy pattern
public:
    LoggerProxy() : deathLogger(new DeathLogger()), abilityLogger(new AbilityLogger()) {}
    ~LoggerProxy() {
        delete deathLogger;
        delete abilityLogger;
    }
    void log(const QString& message) override {
        deathLogger->log(message);
        abilityLogger->log(message);
    }
private:
    DeathLogger* deathLogger;
    AbilityLogger* abilityLogger;
};

#endif // LOGGER_H
