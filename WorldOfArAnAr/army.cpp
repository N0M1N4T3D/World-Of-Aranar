#include "army.h"

Army::Army(IUnitFactory* factory) :
    unitFactory(factory ? factory : new StandardUnitFactory()),
    formationContext(new FormationContext())
{
}

Army::~Army() {
    clear();
    delete unitFactory;
    delete formationContext;
}

void Army::createRandomArmy() {
    clear();
    if (!unitFactory) return;

    for (int i = 0; i < 5; i++) {
        int unitType = QRandomGenerator::global()->bounded(6);
        switch (unitType) {
        case 0: addUnit(unitFactory->createHeavyInfantry()); break;
        case 1: addUnit(unitFactory->createLightInfantry()); break;
        case 2: addUnit(unitFactory->createArcher()); break;
        case 3: addUnit(unitFactory->createDoctor()); break;
        case 4: addUnit(unitFactory->createWizard()); break;
        case 5: addUnit(unitFactory->createGulyayGorod()); break;
        }
    }
}

void Army::addUnit(IUnit* unit) {
    if (unit) units.append(unit);
}

void Army::clear() {
    qDeleteAll(units);
    units.clear();
}

void Army::makeTurn() {
    for (IUnit* unit : units) {
        if (unit->isAlive()) {
            unit->performAction();
        }
    }
}

void Army::attack(Army* enemy, QString& log) {
    IUnit* attacker = getRandomAliveUnit();
    IUnit* defender = enemy ? enemy->getRandomAliveUnit() : nullptr;

    if (!attacker || !defender) return;

    if (attacker->getType() == "Doctor") {
        IUnit* target = getRandomAliveUnit();
        if (target && target->canBeHealed()) {
            target->heal(10);
            log += QString("Doctor heals %1 for 10 HP\n").arg(target->getType());
        }
        return;
    }

    int damage = qMax(1, attacker->getAttack() - defender->getDefense());
    defender->takeDamage(damage);

    log += QString("%1 attacks %2 for %3 damage\n")
               .arg(attacker->getType())
               .arg(defender->getType())
               .arg(damage);

    if (!defender->isAlive()) {
        log += QString("%1 is defeated!\n").arg(defender->getType());
    }
}

bool Army::isDefeated() const {
    return getAliveCount() == 0;
}

int Army::getAliveCount() const {
    int count = 0;
    for (const IUnit* unit : units) {
        if (unit->isAlive()) count++;
    }
    return count;
}

QString Army::getInfo() const {
    QString info;
    for (const IUnit* unit : units) {
        if (unit->isAlive()) {
            info += unit->getInfo() + "\n";
        }
    }
    return info;
}

IUnit* Army::getRandomAliveUnit() const {
    QVector<IUnit*> aliveUnits;
    for (IUnit* unit : units) {
        if (unit->isAlive()) {
            aliveUnits.append(unit);
        }
    }
    return aliveUnits.isEmpty() ? nullptr : aliveUnits[QRandomGenerator::global()->bounded(aliveUnits.size())];
}

void Army::createCustomArmy(const QVector<QString>& unitTypes) {
    clear();
    if (!unitFactory) return;

    for (const QString& type : unitTypes) {
        if (type == "Heavy Infantry") {
            addUnit(unitFactory->createHeavyInfantry());
        } else if (type == "Light Infantry") {
            addUnit(unitFactory->createLightInfantry());
        } else if (type == "Archer") {
            addUnit(unitFactory->createArcher());
        } else if (type == "Doctor") {
            addUnit(unitFactory->createDoctor());
        } else if (type == "Wizard") {
            addUnit(unitFactory->createWizard());
        } else if (type == "Gulyay-Gorod") {
            addUnit(unitFactory->createGulyayGorod());
        }
    }
}


QString Army::getFormationStrategyName() {
    return formationContext->getCurrentStrategyName();
}

void Army::setFormationStrategy(int strategyIndex) {
    formationContext->setStrategyByIndex(strategyIndex);
}

void Army::applyFormationStrategy() {
    if (formationContext && !units.isEmpty()) {
        formationContext->executeStrategy(this);
    }
}
