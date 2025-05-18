#ifndef BUILDER_H
#define BUILDER_H

#include <QVector>
#include "unit.h"
#include "army.h"

class ArmyBuilder { //Builder pattern
public:
    virtual ~ArmyBuilder() = default;

    virtual void buildHeavyInfantry(int count) = 0;
    virtual void buildLightInfantry(int count) = 0;
    virtual void buildArcher(int count) = 0;
    virtual void buildDoctor(int count) = 0;
    virtual void buildWizard(int count) = 0;
    virtual void buildGulyayGorod(int count) = 0;

    virtual Army* getArmy() const = 0;
};

class StandardArmyBuilder : public ArmyBuilder {
public:
    StandardArmyBuilder(IUnitFactory* factory = nullptr);
    void buildHeavyInfantry(int count) override;
    void buildLightInfantry(int count) override;
    void buildArcher(int count) override;
    void buildDoctor(int count) override;
    void buildWizard(int count) override;
    void buildGulyayGorod(int count) override;

    Army* getArmy() const override;

private:
    Army* army;
    IUnitFactory* unitFactory;
};

class ArmyDirector {
public:
    static Army* constructStandardArmy(IUnitFactory* factory = nullptr);
    static Army* constructRandomArmy(IUnitFactory* factory = nullptr, int unitCount = 5);
    static Army* constructCustomArmy(const QVector<QString>& unitTypes, IUnitFactory* factory = nullptr);
};

#endif // BUILDER_H
