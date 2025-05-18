#ifndef STRATEGY_H
#define STRATEGY_H

#include "army.h"
#include <QVector>
#include <algorithm>
class Army;

class IFormationStrategy { //Strategy pattern
public:
    virtual ~IFormationStrategy() = default;
    virtual void arrangeArmy(Army* army) = 0;
    virtual QString getName() const = 0;
};

class LineFormation : public IFormationStrategy {
public:
    void arrangeArmy(Army* army) override;
    QString getName() const override { return "Line Formation"; }
};

class ColumnFormation : public IFormationStrategy {
public:
    void arrangeArmy(Army* army) override;
    QString getName() const override { return "Column Formation"; }
};

class WallFormation : public IFormationStrategy {
public:
    void arrangeArmy(Army* army) override;
    QString getName() const override { return "Wall Formation"; }
};

class FormationContext {
public:
    FormationContext();
    ~FormationContext();

    FormationContext(const FormationContext&) = delete;
    FormationContext& operator=(const FormationContext&) = delete;

    void setStrategy(IFormationStrategy* strategy);
    void executeStrategy(Army* army);
    QString getCurrentStrategyName() const;
    void switchToNextStrategy();
    void setStrategyByIndex(int index);

private:
    IFormationStrategy* currentStrategy;
    QVector<IFormationStrategy*> allStrategies;
    int currentStrategyIndex;
};

#endif // STRATEGY_H
