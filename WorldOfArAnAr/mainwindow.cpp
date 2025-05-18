#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_game(new Game()),
    m_guiManager(new LazyGuiManager(this))
{

    initializeWindow();
    setupConnections();

}


MainWindow::~MainWindow() = default;

void MainWindow::initializeWindow()
{
    m_guiManager->initializeMainWindow(this);
    m_guiManager->initializeStartMenu(this);
    setCentralWidget(m_guiManager->stackedWidget());
    m_guiManager->initializeGameWidgets(m_guiManager->stackedWidget());
    resize(800, 600);
    setWindowTitle("World of ArAnAr");
}

void MainWindow::setupConnections()
{

    connect(m_guiManager->startMenu(), &StartMenu::startGameRequested, this, [this]() {
        m_guiManager->stackedWidget()->setCurrentIndex(1);
        m_guiManager->initializeGameWidgets(m_guiManager->stackedWidget()->widget(1));
        createRandomArmy();
    });
    connect(m_guiManager->startMenu(), &StartMenu::exitRequested,
            qApp, &QApplication::quit);

    connect(m_guiManager->createRandomArmyButton(), &QPushButton::clicked,
            this, &MainWindow::createRandomArmy);
    connect(m_guiManager->createCustomArmyButton(), &QPushButton::clicked,
            this, &MainWindow::createCustomArmy);
    connect(m_guiManager->makeTurnButton(), &QPushButton::clicked,
            this, &MainWindow::makeTurn);
    connect(m_guiManager->makeAllTurnsButton(), &QPushButton::clicked,
            this, &MainWindow::makeAllTurns);
    connect(m_guiManager->saveButton(), &QPushButton::clicked,
            this, &MainWindow::saveGame);
    connect(m_guiManager->loadButton(), &QPushButton::clicked,
            this, &MainWindow::loadGame);
    connect(m_guiManager->showLogButton(), &QPushButton::clicked,
            this, []() { SingletonLogger::getInstance()->showLogWindow(); });
    connect(m_guiManager->exitButton(), &QPushButton::clicked,
            this, &QMainWindow::close);
    connect(m_guiManager->undoButton(), &QPushButton::clicked,
            this, &MainWindow::undo);
    connect(m_guiManager->redoButton(), &QPushButton::clicked,
            this, &MainWindow::redo);

    connect(m_game, &Game::gameUpdated, this, &MainWindow::updateUI);
    connect(m_game->getUndoStack(), &QUndoStack::indexChanged,
            this, &MainWindow::updateUndoRedoButtons);

    connect(m_guiManager->themeSwitchButton(), &QPushButton::clicked, this, [this]() {
        static bool isDark = false;
        if(isDark) {
            switchToStandardStyle();
        } else {
            switchToDarkStyle();
        }
        isDark = !isDark;
    });
}

void MainWindow::createRandomArmy()
{
    m_resultShown = false;
    m_game->resetUndoStack();
    m_game->createArmies();
    m_guiManager->makeTurnButton()->setEnabled(true);
    m_guiManager->makeAllTurnsButton()->setEnabled(true);
    updateUI();
    updateUndoRedoButtons();
}

void MainWindow::createCustomArmy()
{
    m_resultShown = false;
    const int armyBudget = 100;

    UnitSelectionDialog dialog(armyBudget, this);
    if (dialog.exec() == QDialog::Accepted) {
        QVector<QString> army1Units = dialog.getSelectedUnits();
        if (army1Units.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Army 1 must have at least one unit!");
            return;
        }

        UnitSelectionDialog dialog2(armyBudget, this);
        if (dialog2.exec() == QDialog::Accepted) {
            QVector<QString> army2Units = dialog2.getSelectedUnits();
            if (army2Units.isEmpty()) {
                QMessageBox::warning(this, "Warning", "Army 2 must have at least one unit!");
                return;
            }

            m_game->resetUndoStack();
            m_game->createCustomArmies(army1Units, army2Units);
            m_guiManager->makeTurnButton()->setEnabled(true);
            m_guiManager->makeAllTurnsButton()->setEnabled(true);
            updateUI();
            updateUndoRedoButtons();
        }
    }
}

void MainWindow::makeTurn()
{
    if (m_game->isGameOver()) return;

    if (m_game->getUndoStack()->canRedo()) {
        m_game->clearRedoStack();
    }

    TurnCommand* command = new TurnCommand(m_game);
    m_game->getUndoStack()->push(command);
    updateUI();
}

void MainWindow::makeAllTurns()
{
    if (m_game->isGameOver()) return;

    setEnabled(false);
    QApplication::processEvents();

    while (!m_game->isGameOver()) {
        TurnCommand* command = new TurnCommand(m_game);
        m_game->getUndoStack()->push(command);
        QApplication::processEvents();
    }

    setEnabled(true);
    updateUI();
}

void MainWindow::saveGame()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Save Game",
        QDir::homePath() + "/battle.save",
        "Battle Files (*.battle)"
        );
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".battle", Qt::CaseInsensitive)) {
            filename += ".battle";
        }

        m_game->saveGame(filename);
        QMessageBox::information(this, "Success", "Game saved successfully!");
    }
}

void MainWindow::loadGame()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Load Game",
        QDir::homePath(),
        "Battle Files (*.battle)"
        );
    if (!filename.isEmpty()) {
        if (m_game->loadGame(filename)) {
            QMessageBox::information(this, "Success", "Game loaded successfully!");
            updateUI();
        } else {
            QMessageBox::warning(this, "Error", "Failed to load game file!");
        }
    }
}

void MainWindow::undo()
{
    m_game->getUndoStack()->undo();
    updateUI();
}

void MainWindow::redo()
{
    m_game->getUndoStack()->redo();
    updateUI();
}

void MainWindow::updateUndoRedoButtons()
{
    m_guiManager->undoButton()->setEnabled(m_game->getUndoStack()->canUndo());
    m_guiManager->redoButton()->setEnabled(m_game->getUndoStack()->canRedo());
    m_guiManager->undoButton()->setText("Undo");
    m_guiManager->redoButton()->setText("Redo");
}

void MainWindow::updateUI()
{
    updateArmyInfoText();
    updateTurnInfo();
    updateButtonsState();

    if (m_game->isGameOver()) {
        showBattleResult();
    }
}

void MainWindow::updateArmyInfoText()
{

    QString armyInfo = "<html><body style='font-family: Arial;";

    if (dynamic_cast<DarkGUIFactory*>(m_guiManager->getCurrentFactory())) {
        armyInfo += "background-color: #252525; color: white;";
    } else {
        armyInfo += "background-color: white; color: black;";
    }

    auto addUnitHtml = [](QString& html, IUnit* unit) {
        QPixmap pix(unit->getIconPath());
        if (pix.isNull()) pix.load(":/pictures/icons/default_unit.jpg");

        html += QString("<div style='margin: 5px 0;'>"
                        "<img src='%1' width='32' height='32' style='vertical-align: middle; margin-right: 5px;'>"
                        "<span>%2</span></div>")
                    .arg(unit->getIconPath())
                    .arg(unit->getInfo());
    };

    // Army 1
    armyInfo += QString("<h2 style='color: #0066cc;'>Army 1 (%1)</h2>")
                    .arg(m_game->getArmy1()->getFormationStrategyName());
    for (IUnit* unit : m_game->getArmy1()->getUnits()) {
        if (unit->isAlive()) addUnitHtml(armyInfo, unit);
    }

    // Army 2
    armyInfo += QString("<h2 style='color: #cc0000;'>Army 2 (%1)</h2>")
                    .arg(m_game->getArmy2()->getFormationStrategyName());
    for (IUnit* unit : m_game->getArmy2()->getUnits()) {
        if (unit->isAlive()) addUnitHtml(armyInfo, unit);
    }

    armyInfo += "</body></html>";
    m_guiManager->armyInfoText()->setHtml(armyInfo);
}

void MainWindow::updateTurnInfo()
{
    m_guiManager->turnsLeftLabel()->setText(QString("Turn: %1%2")
                                                .arg(m_game->getCurrentTurn())
                                                .arg(m_game->isGameOver() ? " (Game Over)" : ""));
}

void MainWindow::updateButtonsState()
{
    bool gameNotOver = !m_game->isGameOver();
    m_guiManager->makeTurnButton()->setEnabled(gameNotOver);
    m_guiManager->makeAllTurnsButton()->setEnabled(gameNotOver);
    updateUndoRedoButtons();
}

void MainWindow::switchToStandardStyle() {
    m_guiManager->setGUIFactory(new StandardGUIFactory());
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication::setPalette(QApplication::style()->standardPalette());
}

void MainWindow::switchToDarkStyle() {
    m_guiManager->setGUIFactory(new DarkGUIFactory());

    QPalette palette = qApp->palette();
    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(25,25,25));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(palette);

    qApp->setStyle(QStyleFactory::create("Fusion"));
    qApp->setStyleSheet(qApp->styleSheet());
}

void MainWindow::showBattleResult()
{
    if (!m_game->isGameOver() || m_resultShown) {
        return;
    }

    m_resultShown = true;
    QString result = m_game->getBattleResult();

    QMessageBox msgBox;
    msgBox.setWindowTitle("Battle Result");
    msgBox.setText(result);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
