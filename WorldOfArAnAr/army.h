#ifndef ARMY_H
#define ARMY_H

#include <QVector>
#include "unit.h"
#include "strategy.h"

class FormationContext;

class Army
{
public:
    Army(IUnitFactory* factory = nullptr);
    ~Army();

    void setFactory(IUnitFactory* factory) { unitFactory = factory; }
    void createRandomArmy();
    void createCustomArmy(const QVector<QString>& unitTypes);
    void addUnit(IUnit* unit);
    void clear();
    QString getFormationStrategyName();

    void makeTurn();
    void attack(Army* enemy, QString& log);

    bool isDefeated() const;
    int getAliveCount() const;
    QString getInfo() const;

    QVector<IUnit*>& getUnits() { return units; }
    const QVector<IUnit*>& getUnits() const { return units; }
    IUnit* getRandomAliveUnit() const;
    IUnitFactory* getFactory() const { return unitFactory; }

    void applyFormationStrategy();
    void setFormationStrategy(int strategyIndex); // 0-Line, 1-Column, 2-Wall
private:
    FormationContext* formationContext;
    QVector<IUnit*> units;
    IUnitFactory* unitFactory;
};

#endif // ARMY_H
