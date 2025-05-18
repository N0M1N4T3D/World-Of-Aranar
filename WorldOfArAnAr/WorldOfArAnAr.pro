QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abstract_factory_gui.cpp \
    army.cpp \
    builder.cpp \
    game.cpp \
    lazyguimanager.cpp \
    main.cpp \
    mainwindow.cpp \
    singletonlogger.cpp \
    startmenu.cpp \
    strategy.cpp \
    turncommand.cpp \
    turnfacade.cpp \
    unitselectiondialog.cpp

HEADERS += \
    abstract_factory_gui.h \
    army.h \
    builder.h \
    command.h \
    factories/abstractunitfactory.h \
    factories/unitfactory.h \
    game.h \
    lazyguimanager.h \
    logger.h \
    mainwindow.h \
    patterns/adapter.h \
    patterns/decorator.h \
    patterns/objectpool.h \
    patterns/prototype.h \
    patterns/singleton.h \
    singletonlogger.h \
    startmenu.h \
    strategy.h \
    turncommand.h \
    turnfacade.h \
    unit.h \
    unitselectiondialog.h

FORMS += \
    mainwindow.ui \
    startmenu.ui \
    unitselectiondialog.ui

RESOURCES += resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
