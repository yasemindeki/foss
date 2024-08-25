QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QTPLUGIN += QPSQL

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database/sqlprocessor.cpp \
    helpers/filehelper.cpp \
    main.cpp \
    loginscreen.cpp \
    loginform.cpp \
    mainwidget.cpp \
    mainwidgets/qinbox.cpp \
    mainwidgets/qinboxmessagepane.cpp \
    mainwidgets/qpostbrowser.cpp \
    mainwidgets/qpostdesigner.cpp \
    mvc/qfinderitemdelegate.cpp \
    mvc/qfontlistmodel.cpp \
    mvc/qinboxfilterdelegate.cpp \
    mvc/qposttabledelegate.cpp \
    net/restservice.cpp \
    otpform.cpp \
    subwidgets/QAccordionItem.cpp \
    subwidgets/qaccordionwidget.cpp \
    subwidgets/qbuttonlabel.cpp \
    subwidgets/qcategorybutton.cpp \
    subwidgets/qcornergrip.cpp \
    subwidgets/qeventbutton.cpp \
    subwidgets/qfinder.cpp \
    subwidgets/qfinderitem.cpp \
    subwidgets/qfitlabel.cpp \
    subwidgets/qimagebrowser.cpp \
    subwidgets/qinboxbrowser.cpp \
    subwidgets/qinboxfilter.cpp \
    subwidgets/qinboxlistitem.cpp \
    subwidgets/qinboxlistview.cpp \
    subwidgets/qinboxmessagebody.cpp \
    subwidgets/qinboxmessageheader.cpp \
    subwidgets/qinboxtabbar.cpp \
    subwidgets/qinboxtabpane.cpp \
    subwidgets/qinboxtoolbar.cpp \
    subwidgets/qnotificationbox.cpp \
    subwidgets/qpostedit.cpp \
    subwidgets/qposteditblock.cpp \
    subwidgets/qposteditor.cpp \
    subwidgets/qpostwindow.cpp \
    subwidgets/qsidegrip.cpp \
    subwidgets/qsidetabbar.cpp \
    subwidgets/qstyledtoolbar.cpp \
    subwidgets/qtopbarbutton.cpp \
    subwidgets/totextedit.cpp \
    welcomeform.cpp \
    subwidgets/qiconbutton.cpp \
    subwidgets/qlineframe.cpp \
    subwidgets/qlineinput.cpp \
    subwidgets/qtextbutton.cpp \
    windowtitlebar.cpp \
    windowtopbar.cpp \
    ypyscreen.cpp

HEADERS += \
    base/channeltypes.h \
    database/sqlprocessor.h \
    helpers/filehelper.h \
    loginscreen.h \
    loginform.h \
    mainwidget.h \
    mainwidgets/qinbox.h \
    mainwidgets/qinboxmessagepane.h \
    mainwidgets/qpostbrowser.h \
    mainwidgets/qpostdesigner.h \
    mvc/qfinderitemdelegate.h \
    mvc/qfontlistmodel.h \
    mvc/qinboxfilterdelegate.h \
    mvc/qposttabledelegate.h \
    net/restservice.h \
    otpform.h \
    subwidgets/qaccordionitem.h \
    subwidgets/qaccordionwidget.h \
    subwidgets/qbuttonlabel.h \
    subwidgets/qcategorybutton.h \
    subwidgets/qcornergrip.h \
    subwidgets/qeventbutton.h \
    subwidgets/qfinder.h \
    subwidgets/qfinderitem.h \
    subwidgets/qfitlabel.h \
    subwidgets/qimagebrowser.h \
    subwidgets/qinboxbrowser.h \
    subwidgets/qinboxfilter.h \
    subwidgets/qinboxlistitem.h \
    subwidgets/qinboxlistview.h \
    subwidgets/qinboxmessagebody.h \
    subwidgets/qinboxmessageheader.h \
    subwidgets/qinboxtabbar.h \
    subwidgets/qinboxtabpane.h \
    subwidgets/qinboxtoolbar.h \
    subwidgets/qnotificationbox.h \
    subwidgets/qpostedit.h \
    subwidgets/qposteditblock.h \
    subwidgets/qposteditor.h \
    subwidgets/qpostwindow.h \
    subwidgets/qsidegrip.h \
    subwidgets/qsidetabbar.h \
    subwidgets/qstyledtoolbar.h \
    subwidgets/qtopbarbutton.h \
    subwidgets/totextedit.h \
    welcomeform.h \
    subwidgets/qiconbutton.h \
    subwidgets/qlineframe.h \
    subwidgets/qlineinput.h \
    subwidgets/qtextbutton.h \
    windowtitlebar.h \
    windowtopbar.h \
    ypyscreen.h

TRANSLATIONS += \
    yarname_tr_TR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icons/minimize-sign.png

RESOURCES += \
    yarname.qrc

win32: LIBS += -L$$PWD/libs/yncore/windows/ -lyncore
unix:!macx: LIBS += -L$$PWD/libs/yncore/linux/ -lyncore

INCLUDEPATH += $$PWD/libs/yncore/include
DEPENDPATH += $$PWD/libs/yncore/include
