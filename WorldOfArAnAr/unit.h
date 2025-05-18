#ifndef UNIT_H
#define UNIT_H

#include <QString>
#include <QVector>
#include <QRandomGenerator>
#include <QDebug>

class IUnit {
public:
    virtual ~IUnit() = default;
    virtual bool hasBuff(const QString& /*buffName*/) const { return false; }
    virtual QString getType() const = 0;
    virtual int getAttack() const = 0;
    virtual int getDefense() const = 0;
    virtual int getHealth() const = 0;
    virtual int getMaxHealth() const = 0;
    virtual bool isAlive() const = 0;
    virtual bool canBeHealed() const = 0;

    virtual void takeDamage(int damage) = 0;
    virtual void heal(int amount) = 0;
    virtual void performAction() = 0;

    virtual QString getInfo() const = 0;
    virtual IUnit* clone() const = 0; //Prototype pattern
    virtual int getCost() const = 0;

    virtual QString getIconPath() const = 0;
};

class HeavyInfantry : public IUnit {
public:
    HeavyInfantry() : health(100), maxHealth(100) {}

    IUnit* clone() const override { return new HeavyInfantry(*this); }
    QString getType() const override { return "Heavy Infantry"; }
    QString getIconPath() const override { return ":/pictures/units/heavy_infantry.jpg"; }
    int getAttack() const override { return 15; }
    int getDefense() const override { return 10; }
    int getHealth() const override { return health; }
    int getMaxHealth() const override { return maxHealth; }
    bool isAlive() const override { return health > 0; }
    bool canBeHealed() const override { return true; }

    void takeDamage(int damage) override {
        health = qMax(0, health - damage);
    }

    void heal(int amount) override {
        health = qMin(maxHealth, health + amount);
    }

    void performAction() override {
        qDebug() << "Heavy infantry performs powerful attack";
    }

    QString getInfo() const override {
        return QString("%1: ATK %2 DEF %3 HP %4/%5")
            .arg(getType()).arg(getAttack()).arg(getDefense())
            .arg(health).arg(maxHealth);
    }

    int getCost() const override { return 20; }
private:
    int health;
    int maxHealth;
};

class LightInfantry : public IUnit {
public:
    LightInfantry() : health(50), maxHealth(50) {}

    IUnit* clone() const override { return new LightInfantry(*this); }
    QString getType() const override { return "Light Infantry"; }
    QString getIconPath() const override { return ":/pictures/units/light_infantry.jpg"; }
    int getAttack() const override { return 10; }
    int getDefense() const override { return 5; }
    int getHealth() const override { return health; }
    int getMaxHealth() const override { return maxHealth; }
    bool isAlive() const override { return health > 0; }
    bool canBeHealed() const override { return true; }

    void takeDamage(int damage) override {
        health = qMax(0, health - damage);
    }

    void heal(int amount) override {
        health = qMin(maxHealth, health + amount);
    }

    void performAction() override {
        qDebug() << "Light infantry performs quick attack";
    }

    QString getInfo() const override {
        return QString("%1: ATK %2 DEF %3 HP %4/%5")
            .arg(getType()).arg(getAttack()).arg(getDefense())
            .arg(health).arg(maxHealth);
    }

    int getCost() const override { return 10; }
private:
    int health;
    int maxHealth;
};

class Archer : public IUnit {
public:
    Archer() : health(40), maxHealth(40) {}
    IUnit* clone() const override { return new Archer(*this); }
    QString getType() const override { return "Archer"; }
    QString getIconPath() const override { return ":/pictures/units/archer.jpg"; }
    int getAttack() const override { return 20; }
    int getDefense() const override { return 3; }
    int getHealth() const override { return health; }
    int getMaxHealth() const override { return maxHealth; }
    bool isAlive() const override { return health > 0; }
    bool canBeHealed() const override { return true; }

    void takeDamage(int damage) override {
        health = qMax(0, health - damage);
    }

    void heal(int amount) override {
        health = qMin(maxHealth, health + amount);
    }

    void performAction() override {
        qDebug() << "Archer prepares to shoot";
    }

    int getRange() const { return RANGE; }

    QString getInfo() const override {
        return QString("%1: ATK %2 DEF %3 HP %4/%5")
            .arg(getType()).arg(getAttack()).arg(getDefense())
            .arg(health).arg(maxHealth);
    }

    int getCost() const override { return 15; }
private:
    static const int RANGE = 4;
    int health;
    int maxHealth;
};

class Doctor : public IUnit {
public:
    Doctor() : health(60), maxHealth(60) {}

    IUnit* clone() const override { return new Doctor(*this); }
    QString getType() const override { return "Doctor"; }
    QString getIconPath() const override { return ":/pictures/units/doctor.jpg"; }
    int getAttack() const override { return 5; }
    int getDefense() const override { return 3; }
    int getHealth() const override { return health; }
    int getMaxHealth() const override { return maxHealth; }
    bool isAlive() const override { return health > 0; }
    bool canBeHealed() const override { return true; }

    void takeDamage(int damage) override {
        health = qMax(0, health - damage);
    }

    void heal(int amount) override {
        health = qMin(maxHealth, health + amount);
    }


    void performAction() override {
        qDebug() << "Doctor prepares to heal";
    }

    QString getInfo() const override {
        return QString("%1: ATK %2 DEF %3 HP %4/%5")
            .arg(getType()).arg(getAttack()).arg(getDefense())
            .arg(health).arg(maxHealth);
    }

    int getCost() const override { return 25; }
private:
    int health;
    int maxHealth;
};

class Wizard : public IUnit {
public:
    Wizard() : health(70), maxHealth(70) {}

    IUnit* clone() const {
        if (QRandomGenerator::global()->bounded(100) >= 10) {
            return nullptr;
        }
        return nullptr;
    }

    QString getType() const override { return "Wizard"; }
    QString getIconPath() const override { return ":/pictures/units/wizard.jpg"; }
    int getAttack() const override { return 15; }
    int getDefense() const override { return 2; }
    int getHealth() const override { return health; }
    int getMaxHealth() const override { return maxHealth; }
    bool isAlive() const override { return health > 0; }
    bool canBeHealed() const override { return false; }

    void takeDamage(int damage) override {
        health = qMax(0, health - damage);
    }

    void heal(int /*amount*/) override {}

    void performAction() override {
        if (QRandomGenerator::global()->bounded(100) < 30) {
            qDebug() << "Wizard attempts to clone a unit";
        } else {
            qDebug() << "Wizard fails to cast a spell this turn";
        }
    }

    QString getInfo() const override {
        return QString("%1: ATK %2 DEF %3 HP %4/%5")
            .arg(getType()).arg(getAttack()).arg(getDefense())
            .arg(health).arg(maxHealth);
    }

    int getCost() const override { return 30; }

private:
    int health;
    int maxHealth;
};

class IUnitFactory { //Factory Method pattern
public:
    virtual ~IUnitFactory() = default;
    virtual IUnit* createHeavyInfantry() = 0;
    virtual IUnit* createLightInfantry() = 0;
    virtual IUnit* createArcher() = 0;
    virtual IUnit* createDoctor() = 0;
    virtual IUnit* createWizard() = 0;
    virtual IUnit* createGulyayGorod() = 0;
};


class IFortress {
public:
    virtual ~IFortress() = default;
    virtual int getWallHealth() const = 0;
    virtual int getWallDefense() const = 0;
    virtual void damageWall(int damage) = 0;
};

class GulyayGorod : public IFortress {
public:
    GulyayGorod() : health(300), defense(5) {}

    int getWallHealth() const override { return health; }
    int getWallDefense() const override { return defense; }

    void damageWall(int damage) override {
        health = qMax(0, health - damage);
    }

private:
    int health;
    int defense;
};


class GulyayGorodAdapter : public IUnit { //Adapter pattern
public:
    GulyayGorodAdapter(IFortress* fortress) : fortress(fortress) {}

    QString getType() const override { return "Gulyay-Gorod"; }
    QString getIconPath() const override { return ":/pictures/units/gulyay_gorod.jpg"; }
    int getAttack() const override { return 0; }
    int getDefense() const override { return fortress->getWallDefense(); }
    int getHealth() const override { return fortress->getWallHealth(); }
    int getMaxHealth() const override { return 300; }
    bool isAlive() const override { return fortress->getWallHealth() > 0; }
    bool canBeHealed() const override { return false; }

    void takeDamage(int damage) override {
        fortress->damageWall(damage);
    }

    void heal(int /*amount*/) override {
    }

    void performAction() override {
        qDebug() << "Gulyay-Gorod stands as an immovable fortress";
    }

    QString getInfo() const override {
        return QString("%1: DEF %2 HP %3/300 (Fortress)")
            .arg(getType())
            .arg(getDefense())
            .arg(getHealth());
    }

    IUnit* clone() const override {
        return nullptr;
    }


    int getCost() const override { return 40; }
private:
    IFortress* fortress;
};

class StandardUnitFactory : public IUnitFactory {
public:
    IUnit* createHeavyInfantry() override { return new HeavyInfantry(); }
    IUnit* createLightInfantry() override { return new LightInfantry(); }
    IUnit* createArcher() override { return new Archer(); }
    IUnit* createDoctor() override { return new Doctor(); }
    IUnit* createWizard() override { return new Wizard(); }
    IUnit* createGulyayGorod() override { return new GulyayGorodAdapter(new GulyayGorod()); }
};

class EliteUnitFactory : public StandardUnitFactory {
};


class UnitBuffDecorator : public IUnit { //Decorator pattern
protected:
    IUnit* decoratedUnit;
    QMap<QString, QPair<int, int>> buffs;
    QString buffName;
    int attackBonus;
    int defenseBonus;
    bool shouldRemoveBuff;
public:
    UnitBuffDecorator(IUnit* unit, const QString& name, int atk, int def)
        : decoratedUnit(unit) {
        addBuff(name, atk, def);
    }

    bool hasBuff() const { return !buffName.isEmpty(); }
    QString getBuffName() const { return buffName; }
    int getAttackBonus() const { return attackBonus; }
    int getDefenseBonus() const { return defenseBonus; }

    void addBuff(const QString& name, int atk, int def) {
        buffs[name] = qMakePair(atk, def);
    }

    ~UnitBuffDecorator() override {
        delete decoratedUnit;
    }

    QString getType() const override {
        return decoratedUnit->getType();
    }

    int getAttack() const override {
        int total = decoratedUnit->getAttack();
        for (auto it = buffs.constBegin(); it != buffs.constEnd(); ++it) {
            total += it.value().first;
        }
        return total;
    }

    int getDefense() const override {
        int total = decoratedUnit->getDefense();
        for (auto it = buffs.constBegin(); it != buffs.constEnd(); ++it) {
            total += it.value().second;
        }
        return total;
    }

    int getHealth() const override { return decoratedUnit->getHealth(); }
    int getMaxHealth() const override { return decoratedUnit->getMaxHealth(); }
    bool isAlive() const override { return decoratedUnit->isAlive(); }
    bool canBeHealed() const override { return decoratedUnit->canBeHealed(); }
    void takeDamage(int damage) override {
        decoratedUnit->takeDamage(damage);
        if (damage > 5 && QRandomGenerator::global()->bounded(100) < 70) {
            shouldRemoveBuff = true;
        }
    }

    void heal(int amount) override { decoratedUnit->heal(amount); }
    void performAction() override { decoratedUnit->performAction(); }

    QString getInfo() const override {
        QString info = decoratedUnit->getInfo();
        if (!buffs.isEmpty()) {
            info += " [";
            for (auto it = buffs.constBegin(); it != buffs.constEnd(); ++it) {
                if (it != buffs.constBegin()) info += ", ";
                info += QString("%1: ATK+%2 DEF+%3").arg(it.key()).arg(it.value().first).arg(it.value().second);
            }
            info += "]";
        }
        return info;
    }

    IUnit* clone() const override {
        return new UnitBuffDecorator(decoratedUnit->clone(), buffName, attackBonus, defenseBonus);
    }

    int getCost() const override { return decoratedUnit->getCost(); }

    bool needsBuffRemoval() const { return shouldRemoveBuff; }

    IUnit* getBaseUnit() {
        IUnit* base = decoratedUnit;
        decoratedUnit = nullptr;
        return base;
    }

    QString getIconPath() const override {
        return decoratedUnit->getIconPath();
    }

    QString getBuffInfo() const {
        return QString("[%1: ATK+%2 DEF+%3]").arg(buffName).arg(attackBonus).arg(defenseBonus);
    }
};


#endif // UNIT_H
