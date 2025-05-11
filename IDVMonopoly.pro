QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    card.cpp \
    chooserole.cpp \
    gamecell.cpp \
    identity.cpp \
    main.cpp \
    mainwindow.cpp \
    playgame.cpp \
    playrole.cpp \
    statistics.cpp

HEADERS += \
    card.h \
    chooserole.h \
    gamecell.h \
    identity.h \
    mainwindow.h \
    playgame.h \
    playrole.h \
    statistics.h

FORMS += \
    chooserole.ui \
    chooserole.ui \
    chooserole.ui \
    mainwindow.ui \
    playgame.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc \
    image.qrc
