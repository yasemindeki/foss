QT -= gui

CONFIG += c++14 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        rsacrypto.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32: LIBS += -L$$PWD/lib/ -lsqlite3

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib


win32: LIBS += -L$$PWD/lib/openssl/ -llibcrypto.dll

INCLUDEPATH += $$PWD/lib/openssl
DEPENDPATH += $$PWD/lib/openssl

win32: LIBS += -L$$PWD/lib/openssl/ -llibssl.dll

INCLUDEPATH += $$PWD/lib/openssl
DEPENDPATH += $$PWD/lib/openssl

HEADERS += \
    rsacrypto.h
