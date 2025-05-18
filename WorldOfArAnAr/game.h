#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "builder.h"
#include <QDebug>
#include <QUndoStack>
#include "army.h"
#include "turnfacade.h"
#include "logger.h"
#include "builder.h"
#include <QFile>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    ~Game();

    void createArmies(bool eliteFactory = false);
    void createCustomArmies(const QVector<QString>& army1Units, const QVector<QString>& army2Units, bool eliteFactory = false);
    void makeTurn();
    void makeAllTurns();

    QString getArmy1Info() const;
    QString getArmy2Info() const;
    QString getBattleLog() const;
    int getCurrentTurn() const;
    bool isGameOver() const;
    Army* getArmy1() { return army1; }
    Army* getArmy2() { return army2; }
    QString getBattleResult() const;

    bool loadGame(const QString& filename);
    void saveGame(const QString& filename);
    void saveArmy(QDataStream& out, Army* army);
    Army* loadArmy(QDataStream& in);
    void addToLog(const QString &message);

    void setCurrentTurn(int turn) { currentTurn = turn; }
    void setArmies(Army* a1, Army* a2) { army1 = a1; army2 = a2; }
    QUndoStack* getUndoStack() { return &undoStack; }
    void resetUndoStack();
    void clearRedoStack() { undoStack.setIndex(undoStack.index()); }
    void beginMacro(const QString& text) { undoStack.beginMacro(text); }
    void endMacro() { undoStack.endMacro(); }
signals:
    void gameUpdated();

private:
    Army* army1;
    Army* army2;
    int currentTurn;
    LoggerProxy* logger;
    QUndoStack undoStack;
};

#endif // GAME_H
