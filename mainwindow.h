#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "mygraphicsview.h"
#include "mysliderdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void open();
    void save();
    void saveAs();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void openDialog();
    void about();
    void aboutQt();

    void languageEnglish();
    void languageGerman();

    void rValue(int r);
    void gValue(int g);
    void bValue(int b);

    void rgbRadio(bool rgb);
    void yuvRadio(bool yuv);

    void copyImage();
    void pasteImage();

    void openRecentFile();

public:
    void updateStatusBar(QString string);

private:
    void readSettings();
    void writeSettings();

    QString getFileFormat(QString strImageFileName);
    QString originalFileName;
    int saveCancelDiscard();

    MySliderDialog *dialog;
    MyGraphicsView *view;
    QTranslator translatorGerman;

    //layout
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGridLayout *gridLayout;

    //recent files
    enum { MaxRecentFiles = 5 };
    QMenu *recentFilesMenu;
    QAction *recentFileActs[MaxRecentFiles];
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    void loadFile(const QString &fileName);
    QString curFile;
    QString strippedName(const QString &fullFileName);

protected:
    void closeEvent(QCloseEvent *);
    void changeEvent(QEvent *);
};

#endif // MAINWINDOW_H
