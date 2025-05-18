#include "lazyguimanager.h"

LazyGuiManager::LazyGuiManager(QObject *parent) : QObject(parent),
    m_currentFactory(new StandardGUIFactory()) {}

LazyGuiManager::~LazyGuiManager()
{
    delete m_currentFactory;
}

namespace { //графические стили
    const QString DARK_STYLE = "QGroupBox {"
                               "border: 1px solid #555;"
                               "border-radius: 5px;"
                               "margin-top: 1ex;"
                               "background-color: #353535;"
                               "color: white;"
                               "}"
                               "QGroupBox::title {"
                               "subcontrol-origin: margin;"
                               "left: 10px;"
                               "padding: 0 3px;"
                               "color: white;"
                               "}"

                               "QTextEdit {"
                               "background-color: #252525;"
                               "color: white;"
                               "border: 1px solid #555;"
                               "}"

                               "QLabel {"
                               "color: white;"
                               "}";

    const QString STANDARD_STYLE =  "QGroupBox {"
                                   "border: 1px solid #8f8f91;"
                                   "border-radius: 5px;"
                                   "margin-top: 1ex;"
                                   "background-color: #f0f0f0;"
                                   "color: black;"
                                   "}"
                                   "QGroupBox::title {"
                                   "subcontrol-origin: margin;"
                                   "left: 10px;"
                                   "padding: 0 3px;"
                                   "color: black;"
                                   "}"

                                   "QTextEdit {"
                                   "background-color: white;"
                                   "color: black;"
                                   "border: 1px solid #8f8f91;"
                                   "}"

                                   "QLabel {"
                                   "color: black;"
                                   "}";

    const QString GROUP_BOX_STYLE = "QGroupBox { "
                                  "border: 1px solid #ddd; "
                                  "border-radius: 5px; "
                                  "margin-top: 1ex; "
                                  "background-color: %1; "
                                  "}"
                                  "QGroupBox::title { "
                                  "subcontrol-origin: margin; "
                                  "left: 10px; "
                                  "padding: 0 3px; "
                                  "color: %2; "
                                  "}";

    const QString TEXT_EDIT_STYLE = "QTextEdit {"
                                    "background-color: %1;"
                                    "color: %2;"
                                    "border: 1px solid %3;"
                                    "border-radius: 3px;"
                                    "}";

    const QString DARK_STYLE_BOX = "QGroupBox { "
                                   "border: 1px solid #555; "
                                   "border-radius: 5px; "
                                   "margin-top: 1ex; "
                                   "background-color: #353535; "
                                   "}"
                                   "QGroupBox::title { "
                                   "color: white; "
                                   "}";

    const QString DARK_TEXT_STYLE = "QTextEdit { "
                                    "background-color: #252525; "
                                    "color: white; "
                                    "border: 1px solid #555; "
                                    "}";
    const QString STANDARD_STYLE_BOX = "QGroupBox { "
                                       "border: 1px solid #ddd; "
                                       "border-radius: 5px; "
                                       "margin-top: 1ex; "
                                       "background-color: white; "
                                       "}"
                                       "QGroupBox::title { "
                                       "color: black; "
                                       "}";
    const QString STANDARD_TEXT_STYLE = "QTextEdit { "
                                        "background-color: white; "
                                        "color: black; "
                                        "border: 1px solid #ddd; "
                                        "}";
}

void LazyGuiManager::initializeMainWindow(QWidget *parent)
{
    if (m_mainWindowInitialized) return;

    setupStyles();
    m_stackedWidget = new QStackedWidget(parent);
    m_mainWindowInitialized = true;
}

void LazyGuiManager::initializeStartMenu(QWidget *parent)
{
    if (m_startMenuInitialized) return;

    m_startMenu = new StartMenu(parent);
    m_stackedWidget->addWidget(m_startMenu);
    m_startMenuInitialized = true;
}

void LazyGuiManager::initializeGameWidgets(QWidget *parent)
{
    if (m_gameWidgetsInitialized) return;

    m_gameWidget = new QWidget(parent);
    m_mainLayout = new QVBoxLayout(m_gameWidget);

    setupArmyInfoSection(m_gameWidget);
    setupControlSection(m_gameWidget);

    m_stackedWidget->addWidget(m_gameWidget);
    m_gameWidgetsInitialized = true;
}


void LazyGuiManager::setupStyles() {
    QApplication::setStyle("Fusion");

    if (dynamic_cast<DarkGUIFactory*>(m_currentFactory)) {
        qApp->setStyleSheet(DARK_STYLE);
    } else {
        qApp->setStyleSheet(STANDARD_STYLE);
    }
}

void LazyGuiManager::setupArmyInfoSection(QWidget *parent) {
    QGroupBox *armyGroup = new QGroupBox("Army Information", parent);

    QString groupBoxStyle = GROUP_BOX_STYLE;

    if (dynamic_cast<DarkGUIFactory*>(m_currentFactory)) {
        armyGroup->setStyleSheet(groupBoxStyle.arg("#353535").arg("white"));
    } else {
        armyGroup->setStyleSheet(groupBoxStyle.arg("#ffffff").arg("black"));
    }

    QVBoxLayout *armyLayout = new QVBoxLayout(armyGroup);

    m_armyInfoText = new QTextEdit(parent);
    m_armyInfoText->setReadOnly(true);

    QString textEditStyle = TEXT_EDIT_STYLE;

    if (dynamic_cast<DarkGUIFactory*>(m_currentFactory)) {
        m_armyInfoText->setStyleSheet(textEditStyle.arg("#252525").arg("white").arg("#555"));
    } else {
        m_armyInfoText->setStyleSheet(textEditStyle.arg("#ffffff").arg("black").arg("#ddd"));
    }

    armyLayout->addWidget(m_armyInfoText);

    m_turnsLeftLabel = new QLabel("Turn: 0", parent);
    armyLayout->addWidget(m_turnsLeftLabel);

    m_mainLayout->addWidget(armyGroup);
}


void LazyGuiManager::setupControlSection(QWidget *parent) {
    QGroupBox *controlGroup = new QGroupBox("Controls", parent);
    QVBoxLayout *controlLayout = new QVBoxLayout(controlGroup);

    m_createRandomArmyButton = m_currentFactory->createButton("Create Random Armies");
    m_createCustomArmyButton = m_currentFactory->createButton("Create Custom Armies");
    m_makeTurnButton = m_currentFactory->createButton("Make Turn");
    m_makeAllTurnsButton = m_currentFactory->createButton("Make All Turns");
    m_saveButton = m_currentFactory->createButton("Save Game");
    m_loadButton = m_currentFactory->createButton("Load Game");
    m_exitButton = m_currentFactory->createButton("Exit");
    m_undoButton = m_currentFactory->createButton("Undo");
    m_redoButton = m_currentFactory->createButton("Redo");
    m_showLogButton = m_currentFactory->createButton("Show Full Log");
    m_themeSwitchButton = m_currentFactory->createButton("Switch Theme");

    m_makeTurnButton->setProperty("class", "actionButton");
    m_makeAllTurnsButton->setProperty("class", "dangerButton");
    m_exitButton->setProperty("class", "exitButton");
    m_undoButton->setProperty("class", "specialButton");
    m_redoButton->setProperty("class", "specialButton");
    m_showLogButton->setProperty("class", "logButton");

    QHBoxLayout *row1Layout = new QHBoxLayout();
    row1Layout->addWidget(m_createRandomArmyButton);
    row1Layout->addWidget(m_createCustomArmyButton);

    QHBoxLayout *row2Layout = new QHBoxLayout();
    row2Layout->addWidget(m_makeTurnButton);
    row2Layout->addWidget(m_makeAllTurnsButton);

    QHBoxLayout *row3Layout = new QHBoxLayout();
    row3Layout->addWidget(m_saveButton);
    row3Layout->addWidget(m_loadButton);
    row3Layout->addWidget(m_exitButton);

    QHBoxLayout *row4Layout = new QHBoxLayout();
    row4Layout->addWidget(m_undoButton);
    row4Layout->addWidget(m_redoButton);
    row4Layout->addWidget(m_showLogButton);
    row4Layout->addWidget(m_themeSwitchButton);

    controlLayout->addLayout(row1Layout);
    controlLayout->addLayout(row2Layout);
    controlLayout->addLayout(row3Layout);
    controlLayout->addLayout(row4Layout);

    m_mainLayout->addWidget(controlGroup);
}


void LazyGuiManager::setGUIFactory(IGUIFactory* factory) {
    if (m_currentFactory) delete m_currentFactory;
    m_currentFactory = factory;
    applyGUIStyle();
}

void LazyGuiManager::applyGUIStyle() {
    if (!m_currentFactory) return;

    QApplication::setStyle(m_currentFactory->getStyle());

    if (m_gameWidgetsInitialized) {
        QString buttonStyle = m_currentFactory->createButton("")->styleSheet();
        m_createRandomArmyButton->setStyleSheet(buttonStyle);
        m_createCustomArmyButton->setStyleSheet(buttonStyle);
        m_makeTurnButton->setStyleSheet(buttonStyle);
        m_makeAllTurnsButton->setStyleSheet(buttonStyle);
        m_saveButton->setStyleSheet(buttonStyle);
        m_loadButton->setStyleSheet(buttonStyle);
        m_showLogButton->setStyleSheet(buttonStyle);
        m_exitButton->setStyleSheet(buttonStyle);
        m_undoButton->setStyleSheet(buttonStyle);
        m_redoButton->setStyleSheet(buttonStyle);
        m_themeSwitchButton->setStyleSheet(buttonStyle);

        QGroupBox* armyGroup = qobject_cast<QGroupBox*>(m_armyInfoText->parentWidget());
        if (armyGroup) {
            if (dynamic_cast<DarkGUIFactory*>(m_currentFactory)) {
                armyGroup->setStyleSheet(DARK_STYLE_BOX);
                m_armyInfoText->setStyleSheet(DARK_TEXT_STYLE);
                m_turnsLeftLabel->setStyleSheet("QLabel { color: white; }");
            } else {
                armyGroup->setStyleSheet(STANDARD_STYLE_BOX);
                m_armyInfoText->setStyleSheet(STANDARD_TEXT_STYLE);
                m_turnsLeftLabel->setStyleSheet("QLabel { color: black; }");
            }
        }
    }
}
