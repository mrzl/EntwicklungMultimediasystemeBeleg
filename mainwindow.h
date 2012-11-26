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

public:
    void updateStatusBar(QString string);
    int cock;

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

protected:
    virtual void wheelEvent(QWheelEvent *wheelEvent);
};

#endif // MAINWINDOW_H
