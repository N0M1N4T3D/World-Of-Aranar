//lazy pattern

#ifndef LAZYGUIMANAGER_H
#define LAZYGUIMANAGER_H

#include <QObject>
#include <QVector>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QGroupBox>
#include <QApplication>
#include "abstract_factory_gui.h"
#include "startmenu.h"

class StartMenu;
class QStackedWidget;
class QTextEdit;
class QLabel;
class QPushButton;
class QVBoxLayout;

class LazyGuiManager : public QObject
{
    Q_OBJECT
public:
    explicit LazyGuiManager(QObject *parent = nullptr);
    ~LazyGuiManager();

    void initializeMainWindow(QWidget *parent);
    void initializeGameWidgets(QWidget *parent);
    void initializeStartMenu(QWidget *parent);

    void setGUIFactory(IGUIFactory* factory);
    IGUIFactory* getCurrentFactory() const { return m_currentFactory; }
    QPushButton* themeSwitchButton() const { return m_themeSwitchButton; }

    QTextEdit* armyInfoText() const { return m_armyInfoText; }
    QLabel* turnsLeftLabel() const { return m_turnsLeftLabel; }
    QPushButton* createRandomArmyButton() const { return m_createRandomArmyButton; }
    QPushButton* createCustomArmyButton() const { return m_createCustomArmyButton; }
    QPushButton* makeTurnButton() const { return m_makeTurnButton; }
    QPushButton* makeAllTurnsButton() const { return m_makeAllTurnsButton; }
    QPushButton* saveButton() const { return m_saveButton; }
    QPushButton* loadButton() const { return m_loadButton; }
    QPushButton* showLogButton() const { return m_showLogButton; }
    QPushButton* exitButton() const { return m_exitButton; }
    QPushButton* undoButton() const { return m_undoButton; }
    QPushButton* redoButton() const { return m_redoButton; }
    StartMenu* startMenu() const { return m_startMenu; }
    QStackedWidget* stackedWidget() const { return m_stackedWidget; }
    QWidget* gameWidget() const { return m_gameWidget; }

    QVBoxLayout* mainLayout() const { return m_mainLayout; }

private:
    void applyGUIStyle();
    void setupArmyInfoSection(QWidget *parent);
    void setupControlSection(QWidget *parent);
    void setupStyles();

    IGUIFactory* m_currentFactory;


    bool m_mainWindowInitialized = false;
    bool m_gameWidgetsInitialized = false;
    bool m_startMenuInitialized = false;


    QStackedWidget *m_stackedWidget = nullptr;
    QWidget *m_gameWidget = nullptr;
    QTextEdit *m_armyInfoText = nullptr;
    QLabel *m_turnsLeftLabel = nullptr;
    QPushButton *m_createRandomArmyButton = nullptr;
    QPushButton *m_createCustomArmyButton = nullptr;
    QPushButton *m_makeTurnButton = nullptr;
    QPushButton *m_makeAllTurnsButton = nullptr;
    QPushButton *m_saveButton = nullptr;
    QPushButton *m_loadButton = nullptr;
    QPushButton *m_showLogButton = nullptr;
    QPushButton *m_exitButton = nullptr;
    QPushButton *m_undoButton = nullptr;
    QPushButton *m_redoButton = nullptr;
    StartMenu *m_startMenu = nullptr;
    QPushButton *m_themeSwitchButton = nullptr;
    QVBoxLayout *m_mainLayout = nullptr;
};

#endif // LAZYGUIMANAGER_H
