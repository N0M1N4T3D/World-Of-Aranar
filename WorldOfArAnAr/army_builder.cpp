#include "army_builder.h"
#include <QRandomGenerator>

// Реализация StandardArmyBuilder
StandardArmyBuilder::StandardArmyBuilder() :
    factory(std::make_unique<StandardUnitFactory>()),
    currentArmy(nullptr)
{
    reset();
}

StandardArmyBuilder::~StandardArmyBuilder() {
    delete currentArmy; // Удаляем текущую армию, если есть
}

void StandardArmyBuilder::reset() {
    delete currentArmy; // Удаляем предыдущую армию
    currentArmy = new Army(factory.get()); // Создаем новую
}

Army* StandardArmyBuilder::getResult() {
    Army* result = currentArmy;
    currentArmy = nullptr; // Передаем владение
    return result;
}

void StandardArmyBuilder::addHeavyInfantry() {
    currentArmy->addUnit(factory->createHeavyInfantry());
}

void StandardArmyBuilder::addLightInfantry() {
    currentArmy->addUnit(factory->createLightInfantry());
}

void StandardArmyBuilder::addArcher() {
    currentArmy->addUnit(factory->createArcher());
}

void StandardArmyBuilder::addDoctor() {
    currentArmy->addUnit(factory->createDoctor());
}

void StandardArmyBuilder::addWizard() {
    currentArmy->addUnit(factory->createWizard());
}

void StandardArmyBuilder::addGulyayGorod() {
    currentArmy->addUnit(factory->createGulyayGorod());
}


// Реализация ArmyDirector
ArmyDirector::ArmyDirector(IArmyBuilder* builder)
    : builder(builder) {}

void ArmyDirector::setBuilder(IArmyBuilder* newBuilder) {
    builder = newBuilder;
}

Army* ArmyDirector::createRandomArmy(int unitCount) {
    builder->reset();

    for (int i = 0; i < unitCount; ++i) {
        int unitType = QRandomGenerator::global()->bounded(6);
        switch (unitType) {
        case 0: builder->addHeavyInfantry(); break;
        case 1: builder->addLightInfantry(); break;
        case 2: builder->addArcher(); break;
        case 3: builder->addDoctor(); break;
        case 4: builder->addWizard(); break;
        case 5: builder->addGulyayGorod(); break;
        }
    }

    return builder->getResult();
}

Army* ArmyDirector::createCustomArmy(const QVector<QString>& unitTypes) {
    builder->reset();

    for (const QString& type : unitTypes) {
        if (type == "Heavy Infantry") builder->addHeavyInfantry();
        else if (type == "Light Infantry") builder->addLightInfantry();
        else if (type == "Archer") builder->addArcher();
        else if (type == "Doctor") builder->addDoctor();
        else if (type == "Wizard") builder->addWizard();
        else if (type == "Gulyay-Gorod") builder->addGulyayGorod();
    }

    return builder->getResult();
}
