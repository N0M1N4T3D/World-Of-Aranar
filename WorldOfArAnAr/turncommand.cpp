#include "turncommand.h"

TurnCommand::TurnCommand(Game* game) : game(game) {
    saveState(beforeState);
}

TurnCommand::~TurnCommand() {}

void TurnCommand::execute() {
    if (afterState.isEmpty()) {
        game->makeTurn();
        saveState(afterState);
    } else {
        loadState(afterState);
    }
    setText("Turn");
}

void TurnCommand::undo() {
    loadState(beforeState);
}

void TurnCommand::saveState(QByteArray& state) {
    QBuffer buffer(&state);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);

    out << game->getCurrentTurn();
    game->saveArmy(out, game->getArmy1());
    game->saveArmy(out, game->getArmy2());

    buffer.close();
}

void TurnCommand::loadState(const QByteArray& state) {
    QBuffer buffer;
    buffer.setData(state);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    int turn;
    in >> turn;
    game->setCurrentTurn(turn);

    Army* army1 = game->loadArmy(in);
    Army* army2 = game->loadArmy(in);

    delete game->getArmy1();
    delete game->getArmy2();
    game->setArmies(army1, army2);

    buffer.close();
    emit game->gameUpdated();
}
