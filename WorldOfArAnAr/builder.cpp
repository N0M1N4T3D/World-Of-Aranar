#include "builder.h"

StandardArmyBuilder::StandardArmyBuilder(IUnitFactory* factory)
{
    unitFactory = factory ? factory : new StandardUnitFactory();
    army = new Army(unitFactory);
}

void StandardArmyBuilder::buildHeavyInfantry(int count)
{
    for (int i = 0; i < count; ++i) {
        army->addUnit(unitFactory->createHeavyInfantry());
    }
}

void StandardArmyBuilder::buildLightInfantry(int count)
{
    for (int i = 0; i < count; ++i) {
        army->addUnit(unitFactory->createLightInfantry());
    }
}

void StandardArmyBuilder::buildArcher(int count)
{
    for (int i = 0; i < count; ++i) {
        army->addUnit(unitFactory->createArcher());
    }
}

void StandardArmyBuilder::buildDoctor(int count)
{
    for (int i = 0; i < count; ++i) {
        army->addUnit(unitFactory->createDoctor());
    }
}

void StandardArmyBuilder::buildWizard(int count)
{
    for (int i = 0; i < count; ++i) {
        army->addUnit(unitFactory->createWizard());
    }
}

void StandardArmyBuilder::buildGulyayGorod(int count)
{
    for (int i = 0; i < count; ++i) {
        army->addUnit(unitFactory->createGulyayGorod());
    }
}

Army* StandardArmyBuilder::getArmy() const
{
    return army;
}

Army* ArmyDirector::constructStandardArmy(IUnitFactory* factory)
{
    StandardArmyBuilder builder(factory);
    builder.buildHeavyInfantry(2);
    builder.buildLightInfantry(3);
    builder.buildArcher(2);
    builder.buildDoctor(1);
    return builder.getArmy();
}

Army* ArmyDirector::constructRandomArmy(IUnitFactory* factory, int unitCount)
{
    StandardArmyBuilder builder(factory);
    for (int i = 0; i < unitCount; ++i) {
        int unitType = QRandomGenerator::global()->bounded(6);
        switch (unitType) {
        case 0: builder.buildHeavyInfantry(1); break;
        case 1: builder.buildLightInfantry(1); break;
        case 2: builder.buildArcher(1); break;
        case 3: builder.buildDoctor(1); break;
        case 4: builder.buildWizard(1); break;
        case 5: builder.buildGulyayGorod(1); break;
        }
    }
    return builder.getArmy();
}

Army* ArmyDirector::constructCustomArmy(const QVector<QString>& unitTypes, IUnitFactory* factory)
{
    StandardArmyBuilder builder(factory);
    for (const QString& type : unitTypes) {
        if (type == "Heavy Infantry") builder.buildHeavyInfantry(1);
        else if (type == "Light Infantry") builder.buildLightInfantry(1);
        else if (type == "Archer") builder.buildArcher(1);
        else if (type == "Doctor") builder.buildDoctor(1);
        else if (type == "Wizard") builder.buildWizard(1);
        else if (type == "Gulyay-Gorod") builder.buildGulyayGorod(1);
    }
    return builder.getArmy();
}
