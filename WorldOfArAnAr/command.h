#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>

class Game;

class ICommand : public QUndoCommand { //Command pattern
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() override { execute(); }
};

#endif // COMMAND_H
