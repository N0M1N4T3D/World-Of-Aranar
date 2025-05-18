//Abstract Factory pattern

#pragma once

#include <QPushButton>
#include <QStyle>


class IGUIFactory { //Abstract Factory
public:
    virtual ~IGUIFactory() = default;
    virtual QPushButton* createButton(const QString& text) = 0;
    virtual QStyle* getStyle() = 0;
};

class StandardGUIFactory : public IGUIFactory {
public:
    QPushButton* createButton(const QString& text) override;
    QStyle* getStyle() override;
};

class DarkGUIFactory : public IGUIFactory {
public:
    QPushButton* createButton(const QString& text) override;
    QStyle* getStyle() override;
};
