#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T18:06:12
#
#-------------------------------------------------

QT       += core gui network
QT       += webkitwidgets serialport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -L"C:/OpenSSL-Win32" -llibeay32 -lssleay32
INCLUDEPATH += C:/OpenSSL-Win32/include

#QMAKE_LFLAGS = -static-libgcc -static-libstdc++

TARGET = JustConnect
TEMPLATE = app

SOURCES += main.cpp\
        widget.cpp \
    networkhelper.cpp \
    serialdriver.cpp \
    devicemodemdb.cpp \
    devicemodeminfo.cpp \
    modemstatus.cpp \
    devicemodemhuawei.cpp \
    devicemodem.cpp \
    tests.cpp \
    netmonitor.cpp \
    netmonitorworker.cpp \
    uitrayicon.cpp \
    patternparser.cpp \
    uicaptcha.cpp \
    settingsdbutils.cpp \
    mobileprofilesdb.cpp \
    applicationskillerworker.cpp \
    applicationskiller.cpp \
    devicemodemworker.cpp \
    justconnect.cpp \
    uidiagnostics.cpp \
    nicdriver.cpp \
    rasdriver.cpp \
    devicemodemutils.cpp \
    devicemodemsearchingworker.cpp \
    devicemodemportcheckworker.cpp \
    serial.cpp

HEADERS  += widget.h \
    serialdriver.h \
    devicemodemdb.h \
    devicemodeminfo.h \
    modemstatus.h \
    devicemodem.h \
    devicemodemhuawei.h \
    tests.h \
    netmonitor.h \
    netmonitorworker.h \
    uitrayicon.h \
    patternparser.h \
    uicaptcha.h \
    settingsdbutils.h \
    mobileprofilesdb.h \
    applicationskillerworker.h \
    applicationskiller.h \
    devicemodemworker.h \
    justconnect.h \
    uidiagnostics.h \
    nicdriver.h \
    rasdriver.h \
    rasdriverdefs.h \
    devicemodemutils.h \
    devicemodemsearchingworker.h \
    devicemodemportcheckworker.h \
    serial.h

FORMS    += widget.ui \
    uicaptcha.ui \
    uidiagnostics.ui

OTHER_FILES += \
    brown-bunny-rabbit.svg \
    radio_waves.svg \
    antenna_with_bars.svg \
    connectingInternetViaAT.txt \
    Modems.ini \
    MobileApplications.txt \
    MobileProfiles.ini \
    Application.ini \
    readme.md

RESOURCES += \
    resources.qrc

## This sets MY_LIB_FILES the libs you want and should also correctly resolve
## the location of the libs.

win32 {                ## For Windows builds
    # Note: Check to make sure of file name case
    MY_LIB_FILES += $$QT.core.bins/libgcc_s_dw2-1.dll
    MY_LIB_FILES += $$QT.core.bins/libstd~1.dll
    MY_LIB_FILES += $$QT.core.bins/icuin52.dll
    MY_LIB_FILES += $$QT.core.bins/icuuc52.dll
    MY_LIB_FILES += $$QT.core.bins/libwinpthread-1.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Gui.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Core.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5WebKitWidgets.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5SerialPort.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Widgets.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Core.dll
    MY_LIB_FILES += $$QT.core.bins/icudt52.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Network.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5WebKit.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Multimedia.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5MultimediaWidgets.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Sensors.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5OpenGL.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Quick.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5OpenGL.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Qml.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Sql.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5Positioning.dll
    MY_LIB_FILES += $$QT.core.bins/Qt5PrintSupport.dll
}

unix {                     ## For unix builds
    # MY_LIB_FILES += $$QMAKE_LIBDIR_QT/...xxxxxx....
}
CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release
CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/debug

## Define what files are 'extra_libs' and where to put them
extra_libs.files = $$MY_LIB_FILES
extra_libs.path = $$DESTDIR

## Tell qmake to add the moving of them to the 'install' target
INSTALLS += extra_libs
#message(MD: extra_libs.files = $$extra_libs.files)
#message(MD: DESTDIR = $$DESTDIR)
#message(MD: QT.core.bins = $$QT.core.bins)
#message(MD: TARGET = $$TARGET)

