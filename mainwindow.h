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

    void rValue(int r);
    void gValue(int g);
    void bValue(int b);

    void copyImage();
    void pasteImage();

    // recent files
    void openRecentFile();

public:
    void updateStatusBar(QString string);
    void readSettings();
    void writeSettings();

private:
    void createActions();
    void createMenus();
    void updateActions();

    QString getFileFormat(QString strImageFileName);

    QString originalFileName;

    void scaleImage(double scaleFactor);

    MySliderDialog *dialog;
    MyGraphicsView *view;

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGridLayout *gridLayout;

    QAction *openImageAction;
    QAction *saveImageAction;
    QAction *exitAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *normalSizeAction;
    QAction *openDialogAction;
    QAction *aboutAction;
    QAction *aboutQtAction;

    //recent files
    QString curFile;
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
    QMenu *recentFilesMenu;
    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
