#include "strategy.h"


void LineFormation::arrangeArmy(Army* army) {
    QVector<IUnit*>& units = army->getUnits();
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(units.begin(), units.end(), g);
}

void ColumnFormation::arrangeArmy(Army* army) {
    QVector<IUnit*> newOrder;
    QVector<IUnit*> heavies;
    QVector<IUnit*> others;

    for (IUnit* unit : army->getUnits()) {
        if (unit->getType() == "Heavy Infantry" || unit->getType() == "Gulyay-Gorod") {
            heavies.append(unit);
        } else {
            others.append(unit);
        }
    }

    while (!heavies.isEmpty() || !others.isEmpty()) {
        if (!heavies.isEmpty()) {
            newOrder.append(heavies.takeFirst());
        }

        for (int i = 0; i < 2 && !others.isEmpty(); ++i) {
            newOrder.append(others.takeFirst());
        }
    }

    army->getUnits() = newOrder;
}

void WallFormation::arrangeArmy(Army* army) {
    QVector<IUnit*> newOrder;

    for (IUnit* unit : army->getUnits()) {
        if (unit->getType() == "Heavy Infantry" || unit->getType() == "Gulyay-Gorod") {
            newOrder.append(unit);
        }
    }

    for (IUnit* unit : army->getUnits()) {
        if (unit->getType() != "Heavy Infantry" && unit->getType() != "Gulyay-Gorod") {
            newOrder.append(unit);
        }
    }

    army->getUnits() = newOrder;
}

FormationContext::FormationContext() {
    allStrategies.append(new LineFormation());
    allStrategies.append(new ColumnFormation());
    allStrategies.append(new WallFormation());
    currentStrategyIndex = 0;
    currentStrategy = allStrategies.first();
}

FormationContext::~FormationContext() {
    qDeleteAll(allStrategies);
}

void FormationContext::setStrategy(IFormationStrategy* strategy) {
    currentStrategy = strategy;
}

void FormationContext::executeStrategy(Army* army) {
    if (currentStrategy && army) {
        currentStrategy->arrangeArmy(army);
    }
}

QString FormationContext::getCurrentStrategyName() const {
    return currentStrategy ? currentStrategy->getName() : "No Strategy";
}

void FormationContext::switchToNextStrategy() {
    currentStrategyIndex = (currentStrategyIndex + 1) % allStrategies.size();
    currentStrategy = allStrategies[currentStrategyIndex];
}


void FormationContext::setStrategyByIndex(int index) {
    if(index >= 0 && index < allStrategies.size() && index != currentStrategyIndex) {
        currentStrategy = allStrategies[index];
        currentStrategyIndex = index;
    }
}
