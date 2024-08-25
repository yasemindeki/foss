#include "loginscreen.h"
#include "ypyscreen.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QtGui/QFontDatabase>
#include <QDebug>
#include "database/sqlprocessor.h"
#include "subwidgets/qimagebrowser.h"

#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    //qputenv("QT_DEBUG_PLUGINS", QByteArray("1"));
    //qDebug() << QCoreApplication::libraryPaths ();

    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/Goldman-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Goldman-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Ubuntu-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Ubuntu-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Segoe UI.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Segoe UI Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/MerriweatherSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/LucidaTypewriterBold.ttf");

    QImageBrowser::setOptions();

    //SQLProcessor *sqlProcessor = new SQLProcessor();
    //sqlProcessor->exec("QPSQL", "SELECT * FROM contributors");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "yarname_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    LoginScreen *loginWindow = new LoginScreen();
    YPYScreen   mainWindow;

    //mainWindow.hide();
    mainWindow.setLoginWidget(loginWindow);
    mainWindow.show();
    //loginWindow->show();

    return a.exec();
}
