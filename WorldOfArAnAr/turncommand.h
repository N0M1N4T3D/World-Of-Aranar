#ifndef TURNCOMMAND_H
#define TURNCOMMAND_H

#include "command.h"
#include "game.h"
#include <QByteArray>
#include <QBuffer>

class TurnCommand : public ICommand { //Command pattern
public:
    TurnCommand(Game* game);
    ~TurnCommand();

    void execute() override;
    void undo() override;

private:
    Game* game;
    QByteArray beforeState;
    QByteArray afterState;
    void saveState(QByteArray& state);
    void loadState(const QByteArray& state);
};

#endif // TURNCOMMAND_H
