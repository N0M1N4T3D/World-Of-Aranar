#ifndef ARMY_BUILDER_H
#define ARMY_BUILDER_H

#include "army.h"
#include "unit.h"

// Интерфейс строителя армии
class IArmyBuilder {
public:
    virtual ~IArmyBuilder() = default;
    virtual void reset() = 0;
    virtual void addHeavyInfantry() = 0;
    virtual void addLightInfantry() = 0;
    virtual void addArcher() = 0;
    virtual void addDoctor() = 0;
    virtual void addWizard() = 0;
    virtual void addGulyayGorod() = 0;
    virtual Army* getResult() = 0;
};

// Конкретный строитель стандартной армии
class StandardArmyBuilder : public IArmyBuilder {
public:
    StandardArmyBuilder();
    ~StandardArmyBuilder();

    void reset() override;
    void addHeavyInfantry() override;
    void addLightInfantry() override;
    void addArcher() override;
    void addDoctor() override;
    void addWizard() override;
    void addGulyayGorod() override;
    Army* getResult() override;

private:
    Army* currentArmy;
    std::unique_ptr<IUnitFactory> factory;
};

// Директор, который управляет процессом построения
class ArmyDirector {
public:
    ArmyDirector(IArmyBuilder* builder);
    void setBuilder(IArmyBuilder* newBuilder);

    Army* createRandomArmy(int unitCount = 5);
    Army* createCustomArmy(const QVector<QString>& unitTypes);

private:
    IArmyBuilder* builder;
};

#endif // ARMY_BUILDER_H
