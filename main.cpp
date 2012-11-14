#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //initTranslation(a);

    return a.exec();
}

void initTranslation(QApplication app){
    QString translatorName = "Translation_";
    translatorName.append(QLocale::system().name());

    QTranslator translator;
    bool res = translator.load(translatorName, app.applicationDirPath());
    qDebug() << translatorName;
    if(!res){
        qDebug() << "geht nich";
    } else {
        qDebug() << "geht doch";
        QApplication::installTranslator(&translator);
    }
}
