QT += core xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cameraaddition.cpp \
    camerainformation.cpp \
    dbcameraaddition.cpp \
    dynamicchangingcamera.cpp \
    editorscene.cpp \
    main.cpp \
    mainwindow.cpp \
    room.cpp \
    roomcreation.cpp \
    settings.cpp

HEADERS += \
    cameraaddition.h \
    camerainformation.h \
    dbcameraaddition.h \
    dynamicchangingcamera.h \
    editorscene.h \
    mainwindow.h \
    room.h \
    roomcreation.h \
    settings.h

FORMS += \
    cameraaddition.ui \
    camerainformation.ui \
    dbcameraaddition.ui \
    dynamicchangingcamera.ui \
    mainwindow.ui \
    roomcreation.ui \
    settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
