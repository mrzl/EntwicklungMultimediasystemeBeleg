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

/*
 * titanpad: http://titanpad.com/mNrSH0dCdc
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qApp->setWindowIcon(QIcon(qApp->applicationDirPath() + "/" + "icon.ico"));
    return a.exec();
}


