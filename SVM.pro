QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alphabeta.cpp \
    duty.cpp \
    main.cpp \
    mainwindow.cpp \
    svm.cpp \
    vll.cpp \
    vph.cpp \
    wplot/axis.cpp \
    wplot/axisproperties.cpp \
    wplot/cursor.cpp \
    wplot/curve.cpp \
    wplot/iconcursors.cpp \
    wplot/jsonserial.cpp \
    wplot/plotter.cpp \
    wplot/sdata.cpp \
    wplot/state/hzoomstate.cpp \
    wplot/state/normalstate.cpp \
    wplot/state/state.cpp \
    wplot/state/vzoomstate.cpp \
    wplot/state/zoomstate.cpp \
    wplot/wplot.cpp \
    wplot/zoom.cpp

HEADERS += \
    alphabeta.h \
    duty.h \
    mainwindow.h \
    svm.h \
    vll.h \
    vph.h \
    wplot/axis.h \
    wplot/axisproperties.h \
    wplot/cursor.h \
    wplot/curve.h \
    wplot/iconcursors.h \
    wplot/jsonserial.h \
    wplot/plotter.h \
    wplot/sdata.h \
    wplot/state/hzoomstate.h \
    wplot/state/normalstate.h \
    wplot/state/state.h \
    wplot/state/vzoomstate.h \
    wplot/state/zoomstate.h \
    wplot/wplot.h \
    wplot/zoom.h

FORMS += \
    mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    wplot/resource.qrc

DISTFILES += \
    wplot/png/icons8-zoom-in-24.png \
    wplot/png/icons8-zoom-in-30.png \
    wplot/png/icons8-zoom-out-24.png
