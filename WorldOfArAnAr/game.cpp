#include "game.h"


Game::Game(QObject *parent) : QObject(parent), currentTurn(0) {
    army1 = new Army(new StandardUnitFactory());
    army2 = new Army(new StandardUnitFactory());
    logger = new LoggerProxy();
}

Game::~Game() {
    delete army1;
    delete army2;
    delete logger;
}

void Game::addToLog(const QString& message) {
    logger->log(message);
}

void Game::createArmies(bool eliteFactory) {
    delete army1;
    delete army2;

    IUnitFactory* factory = eliteFactory ? new EliteUnitFactory() : nullptr;
    army1 = ArmyDirector::constructRandomArmy(factory);
    army2 = ArmyDirector::constructRandomArmy(factory);

    currentTurn = 0;
    addToLog("=== Battle started! ===");
    SingletonLogger::getInstance()->log("=== Battle started! ===");
    addToLog("Army 1 composition:");
    SingletonLogger::getInstance()->log("Army 1 composition:");
    for (auto unit : army1->getUnits()) {
        SingletonLogger::getInstance()->log(unit->getInfo());
        addToLog(unit->getInfo());
    }
    addToLog("\nArmy 2 composition:");
    SingletonLogger::getInstance()->log("Army 2 composition:");
    for (auto unit : army2->getUnits()) {
        SingletonLogger::getInstance()->log(unit->getInfo());
        addToLog(unit->getInfo());
    }

    emit gameUpdated();
}

void Game::makeTurn() {
    if (isGameOver()) {
        QString result = getBattleResult();
        addToLog("\n=== Battle over! ===");
        addToLog(result);
        SingletonLogger::getInstance()->log("\n=== Battle over! ===");
        SingletonLogger::getInstance()->log(result);
        return;
    }
    currentTurn++;
    SingletonLogger::getInstance()->log(QString("\n--- Turn %1 ---").arg(currentTurn));
    addToLog(QString("\n--- Turn %1 ---").arg(currentTurn));

    TurnFacade turn(army1, army2, *logger);
    turn.executeTurn();

    if (isGameOver()) {
        addToLog("\n=== Battle over! ===");
        SingletonLogger::getInstance()->log("\n=== Battle over! ===");
        QString winner = army1->isDefeated() ? "Army 2 wins!" : "Army 1 wins!";
        addToLog(winner);
        SingletonLogger::getInstance()->log(winner);
    }

    emit gameUpdated();
}

void Game::saveArmy(QDataStream& out, Army* army) {
    int aliveCount = 0;
    for (IUnit* unit : army->getUnits()) {
        if (unit->isAlive()) aliveCount++;
    }
    out << aliveCount;

    for (IUnit* unit : army->getUnits()) {
        if (unit->isAlive()) {
            if (UnitBuffDecorator* buffed = dynamic_cast<UnitBuffDecorator*>(unit)) {
                out << buffed->getType();
                out << buffed->getHealth();
                out << buffed->getBuffName();
                out << buffed->getAttackBonus();
                out << buffed->getDefenseBonus();
            } else {
                out << unit->getType();
                out << unit->getHealth();
                out << QString("");
                out << 0;
                out << 0;
            }
        }
    }
}

Army* Game::loadArmy(QDataStream& in) {
    Army* army = new Army();

    int unitCount;
    in >> unitCount;

    for (int i = 0; i < unitCount; ++i) {
        QString unitType;
        int health;
        QString buffName;
        int atkBonus, defBonus;

        in >> unitType >> health >> buffName >> atkBonus >> defBonus;

        IUnit* unit = nullptr;
        if (unitType == "Heavy Infantry") {
            unit = army->getFactory()->createHeavyInfantry();
        } else if (unitType == "Light Infantry") {
            unit = army->getFactory()->createLightInfantry();
        } else if (unitType == "Archer") {
            unit = army->getFactory()->createArcher();
        } else if (unitType == "Doctor") {
            unit = army->getFactory()->createDoctor();
        } else if (unitType == "Wizard") {
            unit = army->getFactory()->createWizard();
        } else if (unitType == "Gulyay-Gorod") {
            unit = army->getFactory()->createGulyayGorod();
        }

        if (unit) {
            int damage = unit->getMaxHealth() - health;
            if (damage > 0) {
                unit->takeDamage(damage);
            }

            if (!buffName.isEmpty()) {
                unit = new UnitBuffDecorator(unit, buffName, atkBonus, defBonus);
            }

            army->addUnit(unit);
        }
    }

    return army;
}



void Game::saveGame(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for saving:" << filename;
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);

    out << qint32(2);
    out << currentTurn;

    saveArmy(out, army1);
    saveArmy(out, army2);

    file.close();
}

bool Game::loadGame(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for loading:" << filename;
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);

    qint32 version;
    in >> version;

    if (version < 1 || version > 2) {
        qDebug() << "Unsupported save file version:" << version;
        file.close();
        return false;
    }

    resetUndoStack();

    in >> currentTurn;

    // Для старых сохранений (версия 1)
    if (version == 1) {
        delete army1;
        delete army2;
        army1 = new Army();
        army2 = new Army();

        int unitCount;
        for (int army = 0; army < 2; ++army) {
            in >> unitCount;
            for (int i = 0; i < unitCount; ++i) {
                QString unitType;
                int health;
                in >> unitType >> health;

                IUnit* unit = nullptr;
                if (unitType == "Heavy Infantry") {
                    unit = army == 0 ? army1->getFactory()->createHeavyInfantry()
                                     : army2->getFactory()->createHeavyInfantry();
                }

                if (unit) {
                    int damage = unit->getMaxHealth() - health;
                    if (damage > 0) unit->takeDamage(damage);

                    if (army == 0) army1->addUnit(unit);
                    else army2->addUnit(unit);
                }
            }
        }
    }
    else {
        delete army1;
        delete army2;
        army1 = loadArmy(in);
        army2 = loadArmy(in);
    }

    file.close();
    emit gameUpdated();
    return true;
}


QString Game::getArmy1Info() const {
    QString info = "=== Army 1 ===\n" + army1->getInfo();
    if (isGameOver() && !army1->isDefeated()) info += "\n★ WINNER ★";
    return info;
}

QString Game::getArmy2Info() const {
    QString info = "=== Army 2 ===\n" + army2->getInfo();
    if (isGameOver() && !army2->isDefeated()) info += "\n★ WINNER ★";
    return info;
}

int Game::getCurrentTurn() const {
    return currentTurn;
}

bool Game::isGameOver() const {
    if (army1->isDefeated() || army2->isDefeated()) {
        return true;
    }
    if (currentTurn >= 1000) {
        return true;
    }
    return false;
}

QString Game::getBattleResult() const
{
    if (army1->isDefeated() && army2->isDefeated()) {
        return "Draw - both armies are defeated!";
    }
    if (army1->isDefeated()) {
        return "Army 2 wins!";
    }
    if (army2->isDefeated()) {
        return "Army 1 wins!";
    }
    if (currentTurn >= 1000) {
        return QString("Draw after %1 turns!").arg(currentTurn);
    }
    return "Battle continues...";
}

void Game::createCustomArmies(const QVector<QString>& army1Units,
                              const QVector<QString>& army2Units,
                              bool eliteFactory) {
    delete army1;
    delete army2;

    IUnitFactory* factory = eliteFactory ? new EliteUnitFactory() : nullptr;
    army1 = ArmyDirector::constructCustomArmy(army1Units, factory);
    army2 = ArmyDirector::constructCustomArmy(army2Units, factory);

    currentTurn = 0;
    addToLog("=== Battle started! ===");
    addToLog("Army 1 composition:");
    for (auto unit : army1->getUnits()) {
        addToLog(unit->getInfo());
    }
    addToLog("\nArmy 2 composition:");
    for (auto unit : army2->getUnits()) {
        addToLog(unit->getInfo());
    }

    emit gameUpdated();
}

void Game::makeAllTurns() {
    while (!isGameOver()) {
        makeTurn();
    }
}

void Game::resetUndoStack() {
    undoStack.clear();
}
