#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QtGui>


bool loadStyleSheet(QString fileName){
    QFile file(qApp->applicationDirPath() + QDir::separator() + fileName);
    if( !file.open(QFile::ReadOnly) ){
        return false;
    }
    QTextStream stream(&file);
    QString styleSheet = stream.readAll();
    qApp->setStyleSheet(styleSheet);
    return true;
}

void initTranslation(QApplication app){
    QString translatorName = "Translation_";
    translatorName.append(QLocale::system().name());

    QTranslator translator;
    bool res = translator.load(translatorName, app.applicationDirPath());
    qDebug() << translatorName;
    if(!res){
        //qDebug() << "geht nich";
    } else {
        //qDebug() << "geht doch";
        //QApplication::installTranslator(&translator);
        app.installTranslator(&translator);
    }
}

/*
 * titanpad: http://titanpad.com/mNrSH0dCdc
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qApp->setWindowIcon(QIcon(qApp->applicationDirPath() + "/" + "icon.ico"));
    qDebug() << qApp->applicationDirPath() + "/" + "icon.ico";
    //initTranslation(a);
    /*if(loadStyleSheet("style.qss")){
        qDebug() << "Style loaded successfully";
    } else {
        qDebug() << "Could not load Style";
    }*/

    return a.exec();
}


