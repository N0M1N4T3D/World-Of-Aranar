#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QSTyleFactory>
#include "unitselectiondialog.h"
#include "game.h"
#include "lazyguimanager.h"
#include "turncommand.h"
#include "startmenu.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createRandomArmy();
    void createCustomArmy();
    void makeTurn();
    void makeAllTurns();
    void saveGame();
    void loadGame();
    void undo();
    void redo();
    void updateUndoRedoButtons();
    void switchToStandardStyle();
    void switchToDarkStyle();

private:
    void setupConnections();
    void updateUI();
    void updateArmyInfoText();
    void updateTurnInfo();
    void updateButtonsState();
    void initializeWindow();
    void showBattleResult();

    Game* m_game;
    LazyGuiManager* m_guiManager;
    bool m_resultShown = false;
};

#endif // MAINWINDOW_H
