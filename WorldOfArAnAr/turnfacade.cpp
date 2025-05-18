#include "turnfacade.h"

TurnFacade::TurnFacade(Army* army1, Army* army2, LoggerProxy& logger) :
    army1(army1), army2(army2), logger(logger) {}

void TurnFacade::executeTurn() {
    performFirstUnitAttacks();
    applyLightUnitBuffs();
    applyDoctorAbilities();
    applyArcherAbilities();
    applyWizardAbilities();
    removeDeadUnits();
}


void TurnFacade::performFirstUnitAttacks() {
    auto attack = [this](Army* attackerArmy, Army* defenderArmy, const QString& armyName) {
        IUnit* attacker = attackerArmy->getUnits().first();
        IUnit* defender = defenderArmy->getUnits().first();

        if (!attacker->isAlive() || !defender->isAlive()) return;

        SingletonLogger::getInstance()->log(
            QString("%1 %2 (pos 0) attacks %3 %4 (pos 0)")
                .arg(armyName)
                .arg(attacker->getType())
                .arg(defenderArmy == army1 ? "Army1" : "Army2")
                .arg(defender->getType()));

        if (attacker->getType() == "Gulyay-Gorod") {
            SingletonLogger::getInstance()->log("Gulyay-Gorod cannot attack!");
            return;
        }

        int damage = qMax(1, attacker->getAttack() - defender->getDefense());
        defender->takeDamage(damage);

        SingletonLogger::getInstance()->log(
            QString("Deals %1 damage").arg(damage));

        if (!defender->isAlive()) {
            SingletonLogger::getInstance()->log(
                QString("%1 %2 defeated!")
                    .arg(defenderArmy == army1 ? "Army1" : "Army2")
                    .arg(defender->getType()));
        }
    };

    attack(army1, army2, "Army1");
    attack(army2, army1, "Army2");
}

void TurnFacade::applyDoctorAbilities() {
    auto doctorHeal = [this](Army* army, const QString& armyName) {
        QVector<IUnit*>& units = army->getUnits();

        for (int i = 1; i < units.size(); ++i) {
            IUnit* doctor = units[i];
            if (doctor->isAlive() && doctor->getType() == "Doctor") {
                SingletonLogger::getInstance()->log(
                    QString("%1 Doctor (pos %2) attempts to heal")
                        .arg(armyName)
                        .arg(i));

                if (QRandomGenerator::global()->bounded(100) < 30) { // 30% шанс
                    QVector<QPair<IUnit*, int>> possibleTargets;
                    for (int j = 0; j < units.size(); ++j) {
                        IUnit* unit = units[j];
                        if (unit->isAlive() && unit->canBeHealed() &&
                            (unit->getType() == "Light Infantry" ||
                             unit->getType() == "Archer") &&
                            unit->getHealth() < unit->getMaxHealth()) {
                            possibleTargets.append(qMakePair(unit, j));
                        }
                    }

                    if (!possibleTargets.isEmpty()) {
                        auto targetPair = possibleTargets[QRandomGenerator::global()->bounded(possibleTargets.size())];
                        IUnit* target = targetPair.first;
                        int targetPos = targetPair.second;
                        int healAmount = 5 + QRandomGenerator::global()->bounded(6);
                        int oldHealth = target->getHealth();
                        int newHealth = qMin(target->getMaxHealth(), oldHealth + healAmount);

                        target->heal(healAmount);
                        logger.log(QString("%1 Doctor (pos %2) heals %3 (pos %4): %5 HP -> %6 HP (+%7)")
                                       .arg(armyName)
                                       .arg(i)
                                       .arg(target->getType())
                                       .arg(targetPos)
                                       .arg(oldHealth)
                                       .arg(newHealth)
                                       .arg(healAmount));
                        SingletonLogger::getInstance()->log(
                            QString("Healed %1: %2 HP -> %3 HP (+%4)")
                                .arg(target->getType())
                                .arg(oldHealth)
                                .arg(newHealth)
                                .arg(healAmount));
                    } else {
                        logger.log(QString("%1 Doctor (pos %2) found no valid targets")
                                       .arg(armyName)
                                       .arg(i));
                        SingletonLogger::getInstance()->log("No valid targets found");
                    }
                } else {
                    logger.log(QString("%1 Doctor (pos %2) failed healing attempt")
                                   .arg(armyName)
                                   .arg(i));
                    SingletonLogger::getInstance()->log("Healing attempt failed");
                }
            }
        }
    };
    doctorHeal(army1, "Army1");
    doctorHeal(army2, "Army2");
}

void TurnFacade::applyArcherAbilities() {
    auto archerAttack = [this](Army* attackingArmy, Army* defendingArmy, const QString& armyName) {
        QVector<IUnit*>& units = attackingArmy->getUnits();
        std::random_device rd;
        std::mt19937 g(rd());

        for (int i = 1; i < units.size(); ++i) {
            IUnit* archer = units[i];
            if (archer->isAlive() && archer->getType() == "Archer") {
                SingletonLogger::getInstance()->log(
                    QString("%1 Archer (pos %2) preparing to attack")
                        .arg(armyName)
                        .arg(i));

                QVector<IUnit*> enemiesInRange;
                for (IUnit* enemy : defendingArmy->getUnits()) {
                    if (enemy->isAlive()) {
                        enemiesInRange.append(enemy);
                    }
                }

                if (enemiesInRange.size() > 4) {
                    std::shuffle(enemiesInRange.begin(), enemiesInRange.end(), g);
                    enemiesInRange.resize(4);
                    SingletonLogger::getInstance()->log("Limited to 4 random targets");
                }

                if (!enemiesInRange.isEmpty()) {
                    std::uniform_int_distribution<> dist(0, enemiesInRange.size()-1);
                    IUnit* target = enemiesInRange[dist(g)];
                    int damage = qMax(1, archer->getAttack() - target->getDefense());

                    target->takeDamage(damage);
                    logger.log(QString("%1 Archer (pos %2) shoots %3 for %4 damage")
                                   .arg(armyName)
                                   .arg(i)
                                   .arg(target->getType())
                                   .arg(damage));
                    SingletonLogger::getInstance()->log(
                        QString("Successful shot at %1")
                            .arg(target->getType()));

                    if (!target->isAlive()) {
                        logger.log(QString("%1 %2 defeated!")
                                       .arg(defendingArmy == army1 ? "Army1" : "Army2")
                                       .arg(target->getType()));
                        SingletonLogger::getInstance()->log("Target defeated!");
                    }
                } else {
                    logger.log(QString("%1 Archer (pos %2) has no enemies in range")
                                   .arg(armyName)
                                   .arg(i));
                    SingletonLogger::getInstance()->log("No targets in range");
                }
            }
        }
    };
    archerAttack(army1, army2, "Army1");
    archerAttack(army2, army1, "Army2");
}

void TurnFacade::applyWizardAbilities() {
    auto wizardClone = [this](Army* army, const QString& armyName) {
        QVector<IUnit*>& units = army->getUnits();

        for (int i = 1; i < units.size(); ++i) {
            IUnit* wizard = units[i];
            if (wizard->isAlive() && wizard->getType() == "Wizard") {
                SingletonLogger::getInstance()->log(
                    QString("%1 Wizard (pos %2) attempts to clone")
                        .arg(armyName)
                        .arg(i));

                if (QRandomGenerator::global()->bounded(100) < 10) { // 10% шанс
                    if (i > 1 && units[i-1]->isAlive()) {
                        IUnit* unitToClone = units[i-1];

                        if (unitToClone->getType() == "Light Infantry" ||
                            unitToClone->getType() == "Archer" ||
                            unitToClone->getType() == "Doctor") {

                            SingletonLogger::getInstance()->log(
                                QString("Found valid target: %1 (pos %2)")
                                    .arg(unitToClone->getType())
                                    .arg(i-1));

                            IUnit* clone = unitToClone->clone();
                            if (clone) {
                                units.insert(i, clone);
                                logger.log(QString("%1 Wizard (pos %2) cloned %3 (pos %4)")
                                               .arg(armyName)
                                               .arg(i)
                                               .arg(clone->getType())
                                               .arg(i-1));
                                SingletonLogger::getInstance()->log("Clone successful!");
                            }
                        } else {
                            logger.log(QString("%1 Wizard (pos %2) cannot clone %3")
                                           .arg(armyName)
                                           .arg(i)
                                           .arg(unitToClone->getType()));
                            SingletonLogger::getInstance()->log("Invalid clone target");
                        }
                    } else {
                        logger.log(QString("%1 Wizard (pos %2) has no target to clone")
                                       .arg(armyName)
                                       .arg(i));
                        SingletonLogger::getInstance()->log("No target in front");
                    }
                } else {
                    logger.log(QString("%1 Wizard (pos %2) failed clone attempt")
                                   .arg(armyName)
                                   .arg(i));
                    SingletonLogger::getInstance()->log("Clone spell failed");
                }
            }
        }
    };

    wizardClone(army1, "Army1");
    wizardClone(army2, "Army2");
}

void TurnFacade::removeDeadUnits() {
    auto removeFromArmy = [this](Army* army, const QString& armyName) {
        QVector<IUnit*>& units = army->getUnits();
        for (int i = units.size() - 1; i >= 0; --i) {
            if (!units[i]->isAlive()) {
                logger.log(QString("%1 (from %2) removed from battle")
                               .arg(units[i]->getType())
                               .arg(armyName));
                SingletonLogger::getInstance()->log(QString("%1 (from %2) removed from battle")
                                                        .arg(units[i]->getType())
                                                        .arg(armyName));
                delete units[i];
                units.remove(i);
            }
            else if (UnitBuffDecorator* buffed = dynamic_cast<UnitBuffDecorator*>(units[i])) {
                if (buffed->needsBuffRemoval()) {
                    logger.log(QString("%1 (from %2) lost its buff from heavy damage!")
                                   .arg(buffed->getType())
                                   .arg(armyName));
                    SingletonLogger::getInstance()->log(QString("%1 (from %2) lost its buff from heavy damage!")
                                                            .arg(buffed->getType())
                                                            .arg(armyName));
                    IUnit* baseUnit = buffed->getBaseUnit();
                    units[i] = baseUnit;
                    delete buffed;
                }
            }
        }
    };

    removeFromArmy(army1, "Army1");
    removeFromArmy(army2, "Army2");
}

void TurnFacade::applyLightUnitBuffs() {
    auto tryApplyBuff = [this](Army* army, const QString& armyName) {
        QVector<IUnit*>& units = army->getUnits();
        for (int i = 0; i < units.size(); ++i) {
            if (units[i]->isAlive() &&
                (units[i]->getType() == "Heavy Infantry")) {

                bool hasLightNeighbor = false;
                if (i > 0 && units[i-1]->isAlive() && units[i-1]->getType() == "Light Infantry") {
                    hasLightNeighbor = true;
                }
                else if (i < units.size()-1 && units[i+1]->isAlive() &&
                         units[i+1]->getType() == "Light Infantry") {
                    hasLightNeighbor = true;
                }

                if (hasLightNeighbor && QRandomGenerator::global()->bounded(100) < 30) {
                    QString buffType;
                    int atkBonus = 0;
                    int defBonus = 0;

                    QVector<QString> possibleBuffs;
                    if (!units[i]->hasBuff("Helmet")) possibleBuffs << "Helmet";
                    if (!units[i]->hasBuff("Spear")) possibleBuffs << "Spear";
                    if (!units[i]->hasBuff("Shield")) possibleBuffs << "Shield";
                    if (!units[i]->hasBuff("Horse")) possibleBuffs << "Horse";

                    if (!possibleBuffs.isEmpty()) {
                        buffType = possibleBuffs[QRandomGenerator::global()->bounded(possibleBuffs.size())];

                        if (buffType == "Horse") {
                            defBonus = 3;
                        } else if (buffType == "Spear") {
                            atkBonus = 5;
                        } else if (buffType == "Shield") {
                            defBonus = 5;
                        } else if (buffType == "Helmet") {
                            defBonus = 2;
                        }

                        if (UnitBuffDecorator* buffed = dynamic_cast<UnitBuffDecorator*>(units[i])) {
                            buffed->addBuff(buffType, atkBonus, defBonus);
                        } else {
                            units[i] = new UnitBuffDecorator(units[i], buffType, atkBonus, defBonus);
                        }

                        logger.log(QString("%1 (%2) got %3 buff (ATK+%4 DEF+%5)")
                                       .arg(units[i]->getType())
                                       .arg(armyName)
                                       .arg(buffType)
                                       .arg(atkBonus)
                                       .arg(defBonus));
                        SingletonLogger::getInstance()->log(
                            QString("Successfully applied %1 buff to Heavy Infantry (%2)")
                                .arg(buffType).arg(armyName));
                    } else if (hasLightNeighbor) {
                        SingletonLogger::getInstance()->log(
                            QString("Buff attempt failed for Heavy Infantry (%1)")
                                .arg(armyName));
                    }
                }
            }
        }
    };

    tryApplyBuff(army1, "Army1");
    tryApplyBuff(army2, "Army2");
}


void TurnFacade::applyFormationStrategies() {
    QString prevFormation1 = army1->getFormationStrategyName();
    QString prevFormation2 = army2->getFormationStrategyName();

    army1->applyFormationStrategy();
    army2->applyFormationStrategy();

    QString newFormation1 = army1->getFormationStrategyName();
    QString newFormation2 = army2->getFormationStrategyName();

    if (prevFormation1 != newFormation1) {
        logger.log(QString("Army 1 changed formation from %1 to %2")
                       .arg(prevFormation1).arg(newFormation1));
        SingletonLogger::getInstance()->log(QString("Army 1 changed formation from %1 to %2")
                                                .arg(prevFormation1).arg(newFormation1));
    }

    if (prevFormation2 != newFormation2) {
        logger.log(QString("Army 2 changed formation from %1 to %2")
                       .arg(prevFormation2).arg(newFormation2));
        SingletonLogger::getInstance()->log(QString("Army 2 changed formation from %1 to %2")
                                                .arg(prevFormation2).arg(newFormation2));
    }
}
