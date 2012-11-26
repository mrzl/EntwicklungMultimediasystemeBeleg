#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QtGui>
//#include "mainwindow.h"

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    //explicit MyGraphicsView(QWidget *parent = 0);
    explicit MyGraphicsView(QMainWindow *parent);

public slots:
    void rValue(int r);
    void gValue(int g);
    void bValue(int b);

private:
    void getImageFromMimeData(const QMimeData *mimeData);
    int oldR, oldG, oldB;
    QMainWindow *par;
    
public:
    QGraphicsPixmapItem *imageItem;
    QGraphicsPixmapItem *backupItem;
    void setImage(QPixmap map);

    void zoomIn(float factor);
    void zoomOut(float factor);

protected:
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dragMoveEvent(QDragMoveEvent *);
    virtual void dropEvent(QDropEvent *);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // MYGRAPHICSVIEW_H
