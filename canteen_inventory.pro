QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_update.cpp \
    admin_authentication.cpp \
    admin_dash.cpp \
    gen_report.cpp \
    logout.cpp \
    main.cpp \
    mainwindow.cpp \
    manage_inventory.cpp \
    user.cpp \
    user_authentication.cpp

HEADERS += \
    add_update.h \
    admin_authentication.h \
    admin_dash.h \
    gen_report.h \
    logout.h \
    mainwindow.h \
    manage_inventory.h \
    user.h \
    user_authentication.h

FORMS += \
    add_update.ui \
    admin_authentication.ui \
    admin_dash.ui \
    gen_report.ui \
    logout.ui \
    mainwindow.ui \
    manage_inventory.ui \
    user.ui \
    user_authentication.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    b.qrc \
    back.qrc \
    logo.qrc

DISTFILES += \
    back.
