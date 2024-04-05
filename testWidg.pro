QT       += core gui widgets concurrent

CONFIG += c++17
# CONFIG += sanitizer
# CONFIG += sanitize_thread sanitize_undefined
# CONFIG += sanitize_address sanitize_undefined
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pathfinding.cpp \
    usermap.cpp \
    visualmap.cpp \
    visualtile.cpp

HEADERS += \
    mainwindow.h \
    pathfinding.h \
    usermap.h \
    visualmap.h \
    visualtile.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
