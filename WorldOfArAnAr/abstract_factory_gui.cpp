#include "abstract_factory_gui.h"
#include <QPushButton>
#include <QStyleFactory>
#include <QApplication>

namespace {
    const QString STANDARD_BUTTON_STYLE =
    "QPushButton {"
    "background-color: #5a8dee;"
    "color: white;"
    "border: none;"
    "border-radius: 4px;"
    "padding: 6px 12px;"
    "min-width: 80px;"
    "font-weight: 500;"
    "}"
    "QPushButton:hover {"
    "background-color: #4a7dde;"
    "}"
    "QPushButton:pressed {"
    "background-color: #3a6dce;"
    "}"
    "QPushButton:disabled {"
    "background-color: #cccccc;"
    "color: #666666;"
    "}";

    const QString DARK_BUTTON_STYLE =
        "QPushButton {"
        "background-color: #454545;"
        "color: white;"
        "border: 1px solid #555;"
        "border-radius: 4px;"
        "padding: 5px;"
        "min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "background-color: #565656;"
        "}"
        "QPushButton:pressed {"
        "background-color: #353535;"
        "}"
        "QPushButton:disabled {"
        "background-color: #353535;"
        "color: #777;"
        "}";

    QPalette createDarkPalette() {
        QPalette darkPalette;

        darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(35, 35, 35));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ToolTipBase, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);

        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);

        return darkPalette;
    }
}

// Стандартная тема
QPushButton* StandardGUIFactory::createButton(const QString& text) {
    auto btn = new QPushButton(text);
    btn->setStyleSheet(STANDARD_BUTTON_STYLE);
    return btn;
}

QStyle* StandardGUIFactory::getStyle() {
    return QStyleFactory::create("Fusion");
}

// Темная тема
QPushButton* DarkGUIFactory::createButton(const QString& text) {
    auto btn = new QPushButton(text);
    btn->setStyleSheet(DARK_BUTTON_STYLE);
    return btn;
}

QStyle* DarkGUIFactory::getStyle() {
    QStyle* style = QStyleFactory::create("Fusion");
    QApplication::setPalette(createDarkPalette());
    return style;
}
