#ifndef TURNFACADE_H
#define TURNFACADE_H

#include "army.h"
#include "singletonlogger.h"
#include <QString>
#include "logger.h"
#include <algorithm>
#include <random>
#include <QRandomGenerator>


class TurnFacade //Facade pattern
{
public:
    TurnFacade(Army* army1, Army* army2, LoggerProxy& logger);
    void executeTurn();

private:
    void performFirstUnitAttacks();
    void applyDoctorAbilities();
    void applyArcherAbilities();
    void applyWizardAbilities();
    void removeDeadUnits();
    void applyLightUnitBuffs();
    void applyFormationStrategies();

    Army* army1;
    Army* army2;
    LoggerProxy& logger;
};

#endif // TURNFACADE_H
